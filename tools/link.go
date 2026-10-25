// link.go links each module whole, as the original linker did: every
// function's relocatable object and the original bytes between them, in
// address order, with an ASSERT that each name lands at its original address
// (`make link`). The data bytes come from the extracted disc; without it they
// are zeros of the right size, which still proves the code and the layout.
package main

import (
	"bytes"
	"crypto/sha256"
	"debug/elf"
	"encoding/hex"
	"errors"
	"flag"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"sort"
	"strings"
	"sync"
)

// linkPiece is one contiguous range of a module image: a function's .text,
// its .rodata, or the original bytes between them with the data rows inside.
type linkPiece struct {
	addr     uint32
	size     int
	kind     string // "text", "rodata" or "bytes"
	function *functionSpec
	labels   []symbolSpec
}

// linkLayout is a module's pieces in address order; the .rodata copies that
// lie inside another function's (a literal several functions share, which
// each object carries); and the data rows that no bytes piece holds (inside a
// function or past the file), which the link binds to their absolute
// addresses.
type linkLayout struct {
	pieces   []linkPiece
	shadows  []linkPiece
	absolute []symbolSpec
}

// The resident executable starts with its PS-X EXE header (psxEXEHeaderSize
// bytes); the link writes the fields that follow from the layout (the entry
// point, the text address and size) and keeps the rest of the original header.
const mainEntryPoint = "__SN_ENTRY_POINT"

// moduleLinkLayout splits [load, load+size) into the functions' .text and
// .rodata ranges and the bytes between them; MAIN's header is a piece of its
// own.
func moduleLinkLayout(m *moduleSpec) (linkLayout, error) {
	var pieces []linkPiece
	for index := range m.Functions {
		f := &m.Functions[index]
		pieces = append(pieces, linkPiece{addr: f.Addr, size: f.Size, kind: "text", function: f})
		for _, rodata := range f.Rodata {
			pieces = append(pieces, linkPiece{addr: rodata.Addr, size: rodata.Size, kind: "rodata", function: f})
		}
	}
	// The largest of the ranges that start together comes first, so a shared
	// literal's other copies fall inside it.
	sort.Slice(pieces, func(a, b int) bool {
		if pieces[a].addr != pieces[b].addr {
			return pieces[a].addr < pieces[b].addr
		}
		return pieces[a].size > pieces[b].size
	})
	var layout linkLayout
	cursor, end := m.Load, uint32(m.end())
	for _, piece := range pieces {
		if piece.addr < cursor {
			last := layout.pieces[len(layout.pieces)-1]
			if piece.kind == "rodata" && last.kind == "rodata" && piece.addr+uint32(piece.size) <= last.addr+uint32(last.size) {
				layout.shadows = append(layout.shadows, piece)
				continue
			}
			return layout, fmt.Errorf("%s: %s %s at 0x%08x overlaps the range before it", m.ID, piece.function.Name, piece.kind, piece.addr)
		}
		if piece.addr > cursor {
			layout.pieces = append(layout.pieces, linkPiece{addr: cursor, size: int(piece.addr - cursor), kind: "bytes"})
		}
		layout.pieces = append(layout.pieces, piece)
		cursor = piece.addr + uint32(piece.size)
	}
	if cursor > end {
		return layout, fmt.Errorf("%s: its last range ends at 0x%08x, past the file end 0x%08x", m.ID, cursor, end)
	}
	if cursor < end {
		layout.pieces = append(layout.pieces, linkPiece{addr: cursor, size: int(end - cursor), kind: "bytes"})
	}
	if m.ID == mainModuleID {
		first := layout.pieces[0]
		if first.kind != "bytes" || first.addr != m.Load || first.size < psxEXEHeaderSize {
			return layout, fmt.Errorf("%s: no room for the %d-byte PS-X EXE header before its first function", m.ID, psxEXEHeaderSize)
		}
		header := linkPiece{addr: m.Load, size: psxEXEHeaderSize, kind: "header"}
		if first.size == psxEXEHeaderSize {
			layout.pieces[0] = header
		} else {
			rest := linkPiece{addr: m.Load + psxEXEHeaderSize, size: first.size - psxEXEHeaderSize, kind: "bytes"}
			layout.pieces = append([]linkPiece{header, rest}, layout.pieces[1:]...)
		}
	}
	data := append([]symbolSpec(nil), m.Data...)
	sort.Slice(data, func(a, b int) bool { return data[a].Addr < data[b].Addr })
	for _, symbol := range data {
		index := sort.Search(len(layout.pieces), func(i int) bool {
			return layout.pieces[i].addr+uint32(layout.pieces[i].size) > symbol.Addr
		})
		if index < len(layout.pieces) && layout.pieces[index].addr <= symbol.Addr && layout.pieces[index].kind == "bytes" {
			layout.pieces[index].labels = append(layout.pieces[index].labels, symbol)
			continue
		}
		layout.absolute = append(layout.absolute, symbol)
	}
	return layout, nil
}

func linkSectionName(piece linkPiece) string {
	return fmt.Sprintf(".%s_%08x", piece.kind, piece.addr)
}

func linkShadowName(piece linkPiece) string {
	return fmt.Sprintf(".shadow_%08x_%s", piece.addr, piece.function.Name)
}

// linkObjectRenames lists, per function, the symbols its object takes under
// other names in the module link: the definition of a source compiled for
// several entries takes the row's name, and each override names a symbol the
// module resolves to the override's address. An override at an address no
// name resolves to gets a D_<address> label when the address lies in the
// module's bytes (it then moves with them); anywhere else the D_ name is an
// absolute binding, returned with its address.
func linkObjectRenames(resolver symbolResolver, m *moduleSpec, layout *linkLayout) (map[string]map[string]string, map[string]uint32, error) {
	named := map[uint32][]string{}
	addNames := func(names map[string]uint32, keep func(string, uint32) bool) {
		var sorted []string
		for name, addr := range names {
			if keep(name, addr) {
				sorted = append(sorted, name)
			}
		}
		sort.Strings(sorted)
		for _, name := range sorted {
			named[names[name]] = append(named[names[name]], name)
		}
	}
	addNames(resolver.own[m.ID], func(string, uint32) bool { return true })
	for _, module := range append(append([]string(nil), m.Links...), mainModuleID) {
		addNames(resolver.exported[module], func(name string, addr uint32) bool {
			at, ok := resolver.resolve(m, name)
			return ok && at == addr
		})
	}
	generated := map[uint32]string{}
	absolute := map[string]uint32{}
	targetOf := func(addr uint32) string {
		if names := named[addr]; len(names) > 0 {
			return names[0]
		}
		if name, ok := generated[addr]; ok {
			return name
		}
		name := fmt.Sprintf("D_%08x", addr)
		generated[addr] = name
		for index := range layout.pieces {
			piece := &layout.pieces[index]
			if piece.kind == "bytes" && piece.addr <= addr && addr < piece.addr+uint32(piece.size) {
				piece.labels = append(piece.labels, symbolSpec{Addr: addr, Name: name})
				sort.SliceStable(piece.labels, func(a, b int) bool { return piece.labels[a].Addr < piece.labels[b].Addr })
				return name
			}
		}
		absolute[name] = addr
		return name
	}
	renames := map[string]map[string]string{}
	for index := range m.Functions {
		f := &m.Functions[index]
		names := map[string]string{}
		if symbol := f.sourceSymbol(); symbol != f.Name {
			names[symbol] = f.Name
		}
		renames[f.Name] = names
	}
	for _, override := range m.Overrides {
		names := renames[override.Function]
		if names == nil {
			return nil, nil, fmt.Errorf("%s: override of %s names no function %s", m.ID, override.Name, override.Function)
		}
		if at, ok := resolver.resolve(m, override.Name); ok && at == override.Addr {
			continue
		}
		if _, defined := names[override.Name]; defined {
			// A body that names itself by its shared definition already
			// reaches its own entry through the rename.
			if f := m.function(override.Function); f != nil && f.Addr == override.Addr {
				continue
			}
			return nil, nil, fmt.Errorf("%s: %s overrides %s, the name its own definition takes", m.ID, override.Function, override.Name)
		}
		names[override.Name] = targetOf(override.Addr)
	}
	return renames, absolute, nil
}

// linkDataAssembly writes every bytes piece as its own section: the original
// bytes (`.incbin` of the extracted file) or, without it, zeros, with a global
// label at each data row.
func linkDataAssembly(m *moduleSpec, layout linkLayout, image string) string {
	var out strings.Builder
	fmt.Fprintf(&out, "# %s: the bytes between functions, in address order.\n", m.ID)
	for _, piece := range layout.pieces {
		if piece.kind != "bytes" && piece.kind != "header" {
			continue
		}
		fmt.Fprintf(&out, "\t.section %s, \"a\", @progbits\n", linkSectionName(piece))
		at := piece.addr
		emit := func(until uint32) {
			if until == at {
				return
			}
			if image == "" {
				fmt.Fprintf(&out, "\t.space %d\n", until-at)
			} else {
				fmt.Fprintf(&out, "\t.incbin \"%s\", %d, %d\n", image, m.offset(at), until-at)
			}
			at = until
		}
		if piece.kind == "header" {
			// "PS-X EXE", the entry point at 0x10 and the text address and
			// size at 0x18; the other fields and the region text are original.
			fmt.Fprintf(&out, "\t.ascii \"PS-X EXE\"\n")
			at += 8
			emit(piece.addr + 0x10)
			fmt.Fprintf(&out, "\t.word %s\n", mainEntryPoint)
			at += 4
			emit(piece.addr + 0x18)
			fmt.Fprintf(&out, "\t.word __exe_text_start\n\t.word __exe_text_size\n")
			at += 8
		}
		for _, label := range piece.labels {
			emit(label.Addr)
			fmt.Fprintf(&out, "\t.globl %s\n%s:\n", label.Name, label.Name)
		}
		emit(piece.addr + uint32(piece.size))
	}
	return out.String()
}

// linkScript places each piece at its address, binds the names the objects
// leave undefined that this module cannot define (its links, MAIN, imports,
// data rows outside the bytes pieces) and asserts every function and label.
// A module with `$gp`-relative code sets `_gp` to the label gp names.
func linkScript(layout linkLayout, objects map[string]string, dataObject string, bindings map[string]uint32, gp string) string {
	var script strings.Builder
	script.WriteString("OUTPUT_ARCH(mips)\n")
	if gp != "" {
		fmt.Fprintf(&script, "_gp = %s;\n", gp)
	}
	names := make([]string, 0, len(bindings))
	for name := range bindings {
		names = append(names, name)
	}
	sort.Strings(names)
	for _, name := range names {
		fmt.Fprintf(&script, "PROVIDE(%s = 0x%08x);\n", name, bindings[name])
	}
	// The assembler aligns .text to 16 bytes; SUBALIGN keeps each input at its
	// piece's address, as the per-function link does.
	script.WriteString("SECTIONS {\n")
	for _, piece := range layout.pieces {
		switch piece.kind {
		case "text":
			fmt.Fprintf(&script, "  %s 0x%08x : SUBALIGN(4) { \"%s\"(.text) }\n", linkSectionName(piece), piece.addr, objects[piece.function.Name])
		case "rodata":
			fmt.Fprintf(&script, "  %s 0x%08x : SUBALIGN(4) { \"%s\"(.rodata .rdata) }\n", linkSectionName(piece), piece.addr, objects[piece.function.Name])
		default:
			fmt.Fprintf(&script, "  %s 0x%08x : SUBALIGN(1) { \"%s\"(%s) }\n", linkSectionName(piece), piece.addr, dataObject, linkSectionName(piece))
		}
	}
	// Another copy of a shared literal keeps its address for its object's
	// references and loads nothing: the loaded copy holds the same bytes.
	for _, piece := range layout.shadows {
		fmt.Fprintf(&script, "  %s 0x%08x (NOLOAD) : SUBALIGN(4) { \"%s\"(.rodata .rdata) }\n", linkShadowName(piece), piece.addr, objects[piece.function.Name])
	}
	// Every object's .data, .bss and small-data sections are empty
	// (objectHoldsOnlyCode), as is the data object's .text; the debug
	// sections never load.
	fmt.Fprintf(&script, "  /DISCARD/ : { \"%s\"(.text) *(.data) *(.bss) *(.sdata) *(.sbss) *(.reginfo) *(.pdr) *(.comment) *(.gnu.attributes) *(.MIPS.abiflags) *(.debug*) *(.mdebug*) }\n}\n", dataObject)
	if header := layout.pieces[0]; header.kind == "header" && len(layout.pieces) > 1 {
		last := layout.pieces[len(layout.pieces)-1]
		fmt.Fprintf(&script, "__exe_text_start = ADDR(%s);\n", linkSectionName(layout.pieces[1]))
		fmt.Fprintf(&script, "__exe_text_size = ADDR(%s) + SIZEOF(%s) - __exe_text_start;\n", linkSectionName(last), linkSectionName(last))
	}
	for _, piece := range layout.pieces {
		if piece.kind == "text" {
			fmt.Fprintf(&script, "ASSERT(%s == 0x%08x, \"%s must begin at 0x%08x\");\n", piece.function.Name, piece.addr, piece.function.Name, piece.addr)
		}
		for _, label := range piece.labels {
			fmt.Fprintf(&script, "ASSERT(%s == 0x%08x, \"%s must be at 0x%08x\");\n", label.Name, label.Addr, label.Name, label.Addr)
		}
	}
	return script.String()
}

// compileHistoricalObject returns the relocatable object of one source under
// profile in buildDir, from the cache when the same input compiled before.
func (p project) compileHistoricalObject(sourcePath, buildDir string, profile compilerProfile) (string, error) {
	preprocessed, language, err := p.historicalInput(sourcePath, buildDir, profile)
	if err != nil {
		return "", err
	}
	key, err := compilationCacheKey(preprocessed, "", profile, language)
	if err != nil {
		return "", err
	}
	object := filepath.Join(buildDir, profile.name+".o")
	cachePath := filepath.Join(p.root, "build", "cache", "tools", key+".o")
	useCache := os.Getenv("TOOLS_CACHE") != "0"
	if useCache {
		if data, err := os.ReadFile(cachePath); err == nil {
			return object, atomicWrite(object, data)
		}
	}
	if object, err = p.assembleHistoricalObject(sourcePath, preprocessed, buildDir, profile, language); err != nil {
		return "", err
	}
	if useCache {
		data, err := os.ReadFile(object)
		if err != nil {
			return "", err
		}
		if err := os.MkdirAll(filepath.Dir(cachePath), 0o755); err != nil {
			return "", err
		}
		if err := atomicWrite(cachePath, data); err != nil {
			return "", err
		}
	}
	return object, nil
}

type linkResult struct {
	output    string
	identical bool   // the whole image equals the original file
	checked   string // what was compared
}

// linkModule compiles every function of m to an object, writes the data
// assembly and the linker script under build/link/<module>/, links them and
// compares the image with the original file (or, without it, every function
// and rodata range with its hash).
func (p project) linkModule(resolver symbolResolver, m *moduleSpec) (linkResult, error) {
	var result linkResult
	layout, err := moduleLinkLayout(m)
	if err != nil {
		return result, err
	}
	want, imageErr := p.readModuleImage(m)
	if imageErr != nil && !errors.Is(imageErr, errNotExtracted) {
		return result, imageErr
	}
	image := ""
	if imageErr == nil {
		image = filepath.Join(p.filesRoot, filepath.FromSlash(m.File))
	}
	workBase := toolWorkDir()
	if workBase == "" {
		workBase = filepath.Join(p.root, "build", "work")
	}
	workDir := filepath.Join(workBase, "link", m.ID)
	objects := make(map[string]string, len(m.Functions))
	errs := make([]error, len(m.Functions))
	jobs, err := toolJobs()
	if err != nil {
		return result, err
	}
	var lock sync.Mutex
	next := make(chan int)
	var wait sync.WaitGroup
	for range min(jobs, len(m.Functions)) {
		wait.Add(1)
		go func() {
			defer wait.Done()
			for index := range next {
				f := &m.Functions[index]
				profile, _ := compilerProfileNamed(m.profileOf(f))
				source := filepath.Join(p.root, filepath.FromSlash(m.sourceOf(f)))
				object, err := p.compileHistoricalObject(source, filepath.Join(workDir, f.Name), profile)
				lock.Lock()
				objects[f.Name], errs[index] = object, err
				lock.Unlock()
			}
		}()
	}
	for index := range m.Functions {
		next <- index
	}
	close(next)
	wait.Wait()
	if err := errors.Join(errs...); err != nil {
		return result, err
	}

	outDir := filepath.Join(p.root, "build", "link", m.ID)
	if err := os.MkdirAll(filepath.Join(outDir, "objects"), 0o755); err != nil {
		return result, err
	}
	renames, absolute, err := linkObjectRenames(resolver, m, &layout)
	if err != nil {
		return result, err
	}
	for name, object := range objects {
		if err := objectHoldsOnlyCode(name, object); err != nil {
			return result, err
		}
		renamed := filepath.Join(outDir, "objects", name+".o")
		arguments := []string{}
		olds := make([]string, 0, len(renames[name]))
		for old := range renames[name] {
			olds = append(olds, old)
		}
		sort.Strings(olds)
		for _, old := range olds {
			arguments = append(arguments, "--redefine-sym", old+"="+renames[name][old])
		}
		if err := runInDir(p.root, append(append([]string{"mipsel-linux-gnu-objcopy"}, arguments...), object, renamed)...); err != nil {
			return result, err
		}
		objects[name] = renamed
	}
	dataSource := filepath.Join(outDir, "data.s")
	dataObject := filepath.Join(outDir, "data.o")
	if err := atomicWrite(dataSource, []byte(linkDataAssembly(m, layout, image))); err != nil {
		return result, err
	}
	if err := runInDir(p.root, "mipsel-linux-gnu-as", "-EL", "-march=r3000", "-no-pad-sections", "-o", dataObject, dataSource); err != nil {
		return result, err
	}
	defined := map[string]bool{}
	for _, f := range m.Functions {
		defined[f.Name] = true
	}
	for _, piece := range layout.pieces {
		for _, label := range piece.labels {
			defined[label.Name] = true
		}
	}
	bindings := map[string]uint32{}
	for name, addr := range absolute {
		bindings[name] = addr
	}
	for _, object := range objects {
		undefined, err := undefinedObjectIdentifiers(object)
		if err != nil {
			return result, err
		}
		for name := range undefined {
			if addr, ok := resolver.resolve(m, name); ok && !defined[name] {
				bindings[name] = addr
			}
		}
	}
	script := filepath.Join(outDir, "link.ld")
	gp, err := linkGlobalPointer(m, layout)
	if err != nil {
		return result, err
	}
	if err := atomicWrite(script, []byte(linkScript(layout, objects, dataObject, bindings, gp))); err != nil {
		return result, err
	}
	elfPath := filepath.Join(outDir, m.ID+".elf")
	arguments := []string{"-EL", "--orphan-handling=error", "-T", script, "-o", elfPath, dataObject}
	for _, f := range m.Functions {
		arguments = append(arguments, objects[f.Name])
	}
	if output, err := exec.Command("mipsel-linux-gnu-ld", arguments...).CombinedOutput(); err != nil {
		return result, fmt.Errorf("mipsel-linux-gnu-ld: %w\n%s", err, bytes.TrimSpace(output))
	}
	if err := p.checkLinkedSizes(elfPath, layout); err != nil {
		return result, err
	}
	result.output = filepath.Join(outDir, filepath.Base(filepath.FromSlash(m.File)))
	if err := runInDir(p.root, "mipsel-linux-gnu-objcopy", "-O", "binary", elfPath, result.output); err != nil {
		return result, err
	}
	got, err := os.ReadFile(result.output)
	if err != nil {
		return result, err
	}
	if len(got) != m.Size {
		return result, fmt.Errorf("%s: linked image has %d bytes, the file %d", m.ID, len(got), m.Size)
	}
	if want != nil {
		if offset, differs := mismatch(want, got); differs {
			return result, fmt.Errorf("%s: linked image differs at file offset 0x%x (0x%08x)", m.ID, offset, m.Load+uint32(offset))
		}
		sum := sha256.Sum256(got)
		result.identical, result.checked = true, "sha256 "+hex.EncodeToString(sum[:8])
		return result, nil
	}
	checked := 0
	for _, f := range m.Functions {
		if shortHash(got[m.offset(f.Addr):m.offset(f.Addr)+f.Size]) != f.Hash {
			return result, fmt.Errorf("%s: %s differs from its hash", m.ID, f.Name)
		}
		for _, rodata := range f.Rodata {
			if shortHash(got[m.offset(rodata.Addr):m.offset(rodata.Addr)+rodata.Size]) != rodata.Hash {
				return result, fmt.Errorf("%s: %s rodata 0x%08x differs from its hash", m.ID, f.Name, rodata.Addr)
			}
		}
		checked++
	}
	result.checked = fmt.Sprintf("%d function hashes (no disc: data zeroed)", checked)
	return result, nil
}

// linkGlobalPointer names the data label `_gp` takes in a module whose code
// addresses small data relative to `$gp`: the one at the profile's global
// pointer (MAIN's g_psyq_crt_saved_ra, which the C start-up loads). It is ""
// when no function uses such a profile.
func linkGlobalPointer(m *moduleSpec, layout linkLayout) (string, error) {
	var gp uint32
	for index := range m.Functions {
		profile, ok := compilerProfileNamed(m.profileOf(&m.Functions[index]))
		if !ok || profile.smallDataLimit == 0 {
			continue
		}
		if gp != 0 && gp != profile.globalPointer {
			return "", fmt.Errorf("%s: its functions use two global pointers, 0x%08x and 0x%08x", m.ID, gp, profile.globalPointer)
		}
		gp = profile.globalPointer
	}
	if gp == 0 {
		return "", nil
	}
	for _, piece := range layout.pieces {
		for _, label := range piece.labels {
			if label.Addr == gp {
				return label.Name, nil
			}
		}
	}
	return "", fmt.Errorf("%s: no data row in its bytes names the global pointer 0x%08x", m.ID, gp)
}

// objectHoldsOnlyCode fails when a function's object defines data of its own
// (a loaded section other than .text and .rodata with bytes in it, or a
// common symbol): the module's data are its target rows, not C definitions.
func objectHoldsOnlyCode(name, path string) error {
	object, err := elf.Open(path)
	if err != nil {
		return err
	}
	defer object.Close()
	for _, section := range object.Sections {
		if section.Flags&elf.SHF_ALLOC == 0 || section.Size == 0 {
			continue
		}
		switch section.Name {
		case ".text", ".rodata", ".rdata", ".reginfo", ".MIPS.abiflags", ".pdr":
			continue
		}
		return fmt.Errorf("%s defines %d bytes of %s", name, section.Size, section.Name)
	}
	symbols, err := object.Symbols()
	if err != nil {
		return err
	}
	for _, symbol := range symbols {
		if symbol.Section == elf.SHN_COMMON {
			return fmt.Errorf("%s defines the common symbol %s", name, symbol.Name)
		}
	}
	return nil
}

// checkLinkedSizes fails when an object's section is not the size its piece
// declares, which would shift or hide bytes after it.
func (p project) checkLinkedSizes(elfPath string, layout linkLayout) error {
	linked, err := elf.Open(elfPath)
	if err != nil {
		return err
	}
	defer linked.Close()
	for _, piece := range layout.pieces {
		section := linked.Section(linkSectionName(piece))
		switch {
		case section == nil:
			return fmt.Errorf("the link produced no %s", linkSectionName(piece))
		case int(section.Size) != piece.size:
			what := linkSectionName(piece)
			if piece.function != nil {
				what = piece.function.Name + " " + piece.kind
			}
			return fmt.Errorf("%s is %d bytes, the target %d", what, section.Size, piece.size)
		}
	}
	// A shadow loads nothing; `make check` compares each copy's bytes.
	for _, piece := range layout.shadows {
		section := linked.Section(linkShadowName(piece))
		if section == nil || int(section.Size) != piece.size {
			return fmt.Errorf("%s's copy of the literal at 0x%08x is not %d bytes", piece.function.Name, piece.addr, piece.size)
		}
	}
	return nil
}

func (p project) linkCommand(args []string) error {
	flags := flag.NewFlagSet("link", flag.ContinueOnError)
	selection := flags.String("module", "", "modules to link (default: all)")
	if err := flags.Parse(args); err != nil {
		return err
	}
	if flags.NArg() != 0 {
		return errors.New("usage: tools link [--module=M]")
	}
	config, err := p.loadProjectConfig()
	if err != nil {
		return err
	}
	modules := config.Modules
	if *selection != "" {
		if modules, err = resolveModuleSelection(config, *selection); err != nil {
			return err
		}
	}
	resolver := newSymbolResolver(config)
	linked, failed := 0, 0
	for _, m := range modules {
		result, err := p.linkModule(resolver, m)
		if err != nil {
			failed++
			fmt.Fprintf(p.stdout(), "FAIL %s: %s\n", m.ID, strings.ReplaceAll(firstLines(err.Error(), 8), "\n", "\n     "))
			continue
		}
		linked++
		fmt.Fprintf(p.stdout(), "ok   %s (%d bytes, %s)\n", m.ID, m.Size, result.checked)
	}
	fmt.Fprintf(p.stdout(), "link: %d of %d modules linked whole and identical\n", linked, linked+failed)
	if failed > 0 {
		return fmt.Errorf("link: %d modules failed", failed)
	}
	return nil
}
