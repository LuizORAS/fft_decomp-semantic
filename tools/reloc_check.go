// reloc_check.go checks how each module's data words are classified
// (`make reloc-check`, which needs the BIN). A whole word of the bytes
// between functions whose value lies in the game's RAM is a relocation, a
// value or still pending. A pending word fails its module; the pending words
// go to build/reloc-check/<module>.tsv with what the value names. The module
// is then linked twice more with every piece moved, and every word that
// changed must be a relocation moved by the right amount. A pending word that points into the module's own file is an
// orphan: it would keep the old address once the module moves. In code, an
// own address the shifted link left in place (a number in the source, a data
// row bound inside a function, or a linked module's name whose window
// overlaps the module's) is listed for the address-literal and link work.
package main

import (
	"debug/elf"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"
)

// The words reloc-check classifies hold a value in the user RAM above the
// kernel, where every module and fixed game area lies. The kernel's
// 0x80000000-0x8000ffff never moves with the game, and the words that fall
// there in this game are values (two s16 -32768, a sign bit).
const (
	relocScopeLow  = 0x80010000
	relocScopeHigh = 0x80200000
)

// relocCheckShifts are the two moves of the shifted links: the first changes
// only the low halves of the addresses, the second crosses 64 KB, so every
// high half of an address into the module changes too. Both keep every
// alignment up to 16 bytes.
var relocCheckShifts = []uint32{0x10, 0x10010}

// relocFinding is one line of a module's list: a pending data word ("orphan"
// when it points into the module's own file), an own address that code keeps
// in place under a shift ("code", or "link" when a linked module's name is
// bound there), or a data row bound inside a function ("row"), which the
// link places at its absolute address.
type relocFinding struct {
	kind  string
	addr  uint32
	value uint32
	what  string
}

// relocReport counts a module's data words in scope by classification, and
// its disc words and the words outside scope that look like a sector table's
// and are neither disc words nor values ("sector" findings).
type relocReport struct {
	words, relocations, values, pending, orphans int
	disc, discPending                            int
	findings                                     []relocFinding
}

// relocModuleNames is one module's names by address, for describing a value.
type relocModuleNames struct {
	module    *moduleSpec
	functions []functionSpec    // by address
	rows      map[uint32]string // the first data row name at each address
	names     []symbolSpec      // functions and data rows, by address
}

// relocNames describes what an address names, for the pending list, and
// which disc file each first sector starts.
type relocNames struct {
	config  *projectConfig
	modules map[string]relocModuleNames
	starts  map[uint32]string
}

func newRelocNames(config *projectConfig) relocNames {
	names := relocNames{config: config, modules: make(map[string]relocModuleNames, len(config.Modules)), starts: make(map[uint32]string)}
	for name, file := range config.discFiles {
		names.starts[uint32(file.lba)] = name
	}
	for _, m := range config.Modules {
		entry := relocModuleNames{module: m, rows: make(map[uint32]string)}
		entry.functions = append([]functionSpec(nil), m.Functions...)
		sort.Slice(entry.functions, func(a, b int) bool { return entry.functions[a].Addr < entry.functions[b].Addr })
		data := append([]symbolSpec(nil), m.Data...)
		sortSymbols(data)
		for _, symbol := range data {
			if _, ok := entry.rows[symbol.Addr]; !ok {
				entry.rows[symbol.Addr] = symbol.Name
			}
		}
		for _, f := range entry.functions {
			entry.names = append(entry.names, symbolSpec{Addr: f.Addr, Name: f.Name})
		}
		entry.names = append(entry.names, data...)
		sort.SliceStable(entry.names, func(a, b int) bool { return entry.names[a].Addr < entry.names[b].Addr })
		names.modules[m.ID] = entry
	}
	return names
}

// describe says what value names in m's resolution order (itself, its links,
// main): a function's start or a place inside it, a data row, or a place past
// the last name below it in that module's file; otherwise which other
// modules' files hold it. Names of another module carry its id.
func (n relocNames) describe(m *moduleSpec, value uint32) string {
	order := append([]string{m.ID}, m.Links...)
	if m.ID != mainModuleID {
		order = append(order, mainModuleID)
	}
	prefix := func(id string) string {
		if id == m.ID {
			return ""
		}
		return id + " "
	}
	for _, id := range order {
		names := n.modules[id]
		functions := names.functions
		index := sort.Search(len(functions), func(i int) bool { return functions[i].Addr > value }) - 1
		switch {
		case index >= 0 && functions[index].Addr == value:
			return prefix(id) + "function " + functions[index].Name
		case index >= 0 && value < functions[index].Addr+uint32(functions[index].Size):
			return fmt.Sprintf("%sinside %s+0x%x", prefix(id), functions[index].Name, value-functions[index].Addr)
		}
		if row, ok := names.rows[value]; ok {
			return prefix(id) + "data " + row
		}
	}
	inOrder := map[string]bool{}
	for _, id := range order {
		inOrder[id] = true
		names, ok := n.modules[id]
		if !ok || !names.module.contains(value, 1) {
			continue
		}
		all := names.names
		if index := sort.Search(len(all), func(i int) bool { return all[i].Addr > value }) - 1; index >= 0 {
			return fmt.Sprintf("%spast %s+0x%x", prefix(id), all[index].Name, value-all[index].Addr)
		}
	}
	var files []string
	for _, other := range n.config.Modules {
		if !inOrder[other.ID] && other.contains(value, 1) {
			files = append(files, other.ID)
		}
	}
	switch len(files) {
	case 0:
		return "nothing"
	case 1:
		return "in " + files[0] + "'s file"
	}
	return fmt.Sprintf("in %d modules' files (%s, ...)", len(files), files[0])
}

// discSector reports which file a data word of piece looks like a sector table
// entry of: the file's first sector followed by its size rounded up to whole
// sectors (an extent of a file that is not empty), or one of three or more
// first sectors in a row.
func (n relocNames) discSector(m *moduleSpec, piece linkPiece, original []byte, addr uint32) (string, bool) {
	word := func(at uint32) (uint32, bool) {
		if at < piece.addr || at+4 > piece.addr+uint32(piece.size) {
			return 0, false
		}
		return binary.LittleEndian.Uint32(original[m.offset(at):]), true
	}
	value, _ := word(addr)
	name, ok := n.starts[value]
	if !ok {
		return "", false
	}
	file := n.config.discFiles[name]
	rounded := uint32((file.size + discDataSectorBytes - 1) / discDataSectorBytes * discDataSectorBytes)
	if next, ok := word(addr + 4); ok && file.size > 0 && next == rounded {
		return name, true
	}
	start := func(at uint32) bool {
		other, ok := word(at)
		_, starts := n.starts[other]
		return ok && starts
	}
	run := 1
	for at := addr + 4; run < 3 && start(at); at += 4 {
		run++
	}
	for at := addr - 4; run < 3 && start(at); at -= 4 {
		run++
	}
	return name, run >= 3
}

// relocClassify sorts m's data words in scope (whole words of the bytes
// pieces) into relocations, values and pending, and its disc words, and fails
// when a relocation does not name the address its original word holds or a
// disc word differs from what the layout gives. A word outside scope that
// looks like a sector table's entry (discSector) and is neither a disc word
// nor a value is a "sector" finding.
func relocClassify(resolver symbolResolver, names relocNames, m *moduleSpec, layout linkLayout, original []byte) (relocReport, error) {
	var report relocReport
	relocations := make(map[uint32]relocationSpec, len(m.Relocations))
	for _, relocation := range m.Relocations {
		relocations[relocation.Addr] = relocation
	}
	values := make(map[uint32]bool)
	for _, value := range m.Values {
		for addr := value.Addr; addr < value.end(); addr += 4 {
			values[addr] = true
		}
	}
	disc := make(map[uint32]uint32)
	for _, word := range m.DiscWords {
		for index, value := range word.values {
			disc[word.Addr+uint32(4*index)] = value
		}
	}
	var wrong []string
	for _, piece := range layout.pieces {
		if piece.kind != "bytes" {
			continue
		}
		for addr := (piece.addr + 3) &^ 3; addr+4 <= piece.addr+uint32(piece.size); addr += 4 {
			value := binary.LittleEndian.Uint32(original[m.offset(addr):])
			inScope := relocScopeLow <= value && value < relocScopeHigh
			if layoutValue, ok := disc[addr]; ok {
				if layoutValue != value {
					wrong = append(wrong, fmt.Sprintf("disc word 0x%08x holds %d, but the layout gives %d", addr, value, layoutValue))
				}
				report.disc++
				continue
			}
			if !inScope && !values[addr] {
				if file, ok := names.discSector(m, piece, original, addr); ok {
					report.discPending++
					report.findings = append(report.findings, relocFinding{kind: "sector", addr: addr, value: value, what: "first sector of " + file})
				}
			}
			if relocation, ok := relocations[addr]; ok {
				if to, _ := resolver.resolve(m, relocation.To); to+relocation.Offset != value {
					wrong = append(wrong, fmt.Sprintf("relocation 0x%08x holds 0x%08x, but %s+0x%x is 0x%08x", addr, value, relocation.To, relocation.Offset, to+relocation.Offset))
				}
				if inScope {
					report.words++
					report.relocations++
				}
				continue
			}
			if !inScope {
				continue
			}
			report.words++
			if values[addr] {
				report.values++
				continue
			}
			report.pending++
			kind := "pending"
			if m.contains(value, 1) {
				kind = "orphan"
				report.orphans++
			}
			report.findings = append(report.findings, relocFinding{kind: kind, addr: addr, value: value, what: names.describe(m, value)})
		}
	}
	if len(wrong) > 0 {
		return report, fmt.Errorf("%s: %s", m.ID, strings.Join(wrong[:min(len(wrong), 3)], "; "))
	}
	return report, nil
}

// addressHalfOpcodes are the instructions that complete a lui's address with
// a low half: addi, addiu, ori and every load and store.
var addressHalfOpcodes = map[uint32]bool{
	0x08: true, 0x09: true, 0x0d: true,
	0x20: true, 0x21: true, 0x22: true, 0x23: true, 0x24: true, 0x25: true, 0x26: true,
	0x28: true, 0x29: true, 0x2a: true, 0x2b: true, 0x2e: true, 0x32: true, 0x3a: true,
}

// shiftedInstructionExplained reports whether an instruction a link shifted
// by shift changed as a relocation moves it: a jump's target field by the
// words moved, a lui's high half by the shift's (plus the low half's carry),
// an address half's 16-bit offset or immediate by the shift's low half.
func shiftedInstructionExplained(before, after, shift uint32) bool {
	op := before >> 26
	switch {
	case after>>26 != op:
		return false
	case op == 2 || op == 3: // j, jal
		return (after-before)&0x3ffffff == (shift>>2)&0x3ffffff
	case op == 0x0f: // lui
		delta := (after - before) & 0xffff
		return after>>16 == before>>16 && (delta == (shift>>16)&0xffff || delta == (shift>>16+1)&0xffff)
	}
	return addressHalfOpcodes[op] && after>>16 == before>>16 && (after-before)&0xffff == shift&0xffff
}

// pairedAddress is the address the lui at image[offset] builds with the next
// instruction (within 12, before end) that completes it from the same
// register.
func pairedAddress(image []byte, offset, end int) (uint32, bool) {
	lui := binary.LittleEndian.Uint32(image[offset:])
	register := lui >> 16 & 31
	for next := offset + 4; next+4 <= min(end, offset+52); next += 4 {
		word := binary.LittleEndian.Uint32(image[next:])
		if word>>21&31 != register || !addressHalfOpcodes[word>>26] {
			continue
		}
		low := word & 0xffff
		if word>>26 == 0x0d { // ori
			return lui<<16 | low, true
		}
		return lui<<16 + uint32(int32(int16(low))), true
	}
	return 0, false
}

// relocCompareShifted compares m's image linked shifted by shift with the
// original, word by word. A changed word must be explained: an instruction's
// jump target or address half moved by shift (shiftedInstructionExplained),
// a .rodata or header word moved by it, or a relocated data word moved as
// far as its symbol (moved). It returns the unexplained changes and, for a
// shift past 64 KB, the instructions that still name an address in m's own
// file: a number in the source or one of m's data rows the link binds inside
// code (rows), and, as "link", a linked module's name bound there (foreign),
// which lies in m's file because the two windows overlap.
func relocCompareShifted(m *moduleSpec, layout linkLayout, original, shifted []byte, shift uint32, moved func(string) (uint32, bool), rows, foreign map[uint32]string) ([]string, []relocFinding) {
	relocations := make(map[uint32]relocationSpec, len(m.Relocations))
	for _, relocation := range m.Relocations {
		relocations[relocation.Addr] = relocation
	}
	var unexplained []string
	var code []relocFinding
	for _, piece := range layout.pieces {
		for addr := (piece.addr + 3) &^ 3; addr+4 <= piece.addr+uint32(piece.size); addr += 4 {
			before := binary.LittleEndian.Uint32(original[m.offset(addr):])
			after := binary.LittleEndian.Uint32(shifted[m.offset(addr):])
			explained := true
			switch piece.kind {
			case "text":
				if before != after {
					explained = shiftedInstructionExplained(before, after, shift)
				} else if shift >= 0x10000 {
					if finding, ok := fixedOwnAddress(m, piece, original, addr, before); ok {
						if name, ok := rows[finding.value]; ok {
							finding.what += ", bound to " + name
						} else if name, ok := foreign[finding.value]; ok {
							finding.kind = "link"
							finding.what += ", bound to " + name
						}
						code = append(code, finding)
					}
				}
			case "bytes":
				want := before
				if relocation, ok := relocations[addr]; ok {
					delta, ok := moved(relocation.To)
					explained = ok
					want += delta
				}
				explained = explained && after == want
			default: // .rodata and MAIN's header
				explained = before == after || after-before == shift
			}
			if !explained {
				unexplained = append(unexplained, fmt.Sprintf("0x%08x %s 0x%08x -> 0x%08x", addr, piece.kind, before, after))
			}
		}
	}
	return unexplained, code
}

// fixedOwnAddress reports an instruction that a link shifted past 64 KB left
// in place although it names an address in m's own file: a jump into it, or
// a lui of such a high half, with the address its paired instruction
// completes (or none found).
func fixedOwnAddress(m *moduleSpec, piece linkPiece, image []byte, addr, word uint32) (relocFinding, bool) {
	where := fmt.Sprintf("%s+0x%x", piece.function.Name, addr-piece.addr)
	switch word >> 26 {
	case 2, 3:
		target := (addr+4)&0xf0000000 | (word&0x3ffffff)<<2
		return relocFinding{kind: "code", addr: addr, value: target, what: where + " jump"}, m.contains(target, 1)
	case 0x0f:
		// MAIN's file starts in its header, below the scope: a lui of 0x8000
		// there is the constant 0x80000000.
		high := word & 0xffff
		if high < max(m.Load, relocScopeLow)>>16 || high > (uint32(m.end())-1+0x8000)>>16 {
			return relocFinding{}, false
		}
		full, ok := pairedAddress(image, m.offset(addr), m.offset(piece.addr)+piece.size)
		if !ok {
			return relocFinding{kind: "code", addr: addr, value: high << 16, what: where + " lui with no paired use"}, true
		}
		return relocFinding{kind: "code", addr: addr, value: full, what: where}, m.contains(full, 1)
	}
	return relocFinding{}, false
}

// relocRows lists m's data rows that the link binds at an absolute address
// inside the module's own code: inside a function's .text or a .rodata.
func relocRows(m *moduleSpec, layout linkLayout) []relocFinding {
	var rows []relocFinding
	for _, symbol := range layout.absolute {
		if !m.contains(symbol.Addr, 1) {
			continue
		}
		for _, piece := range layout.pieces {
			if piece.addr <= symbol.Addr && symbol.Addr < piece.addr+uint32(piece.size) && piece.function != nil {
				rows = append(rows, relocFinding{kind: "row", addr: symbol.Addr, what: fmt.Sprintf("%s in %s .%s+0x%x", symbol.Name, piece.function.Name, piece.kind, symbol.Addr-piece.addr)})
				break
			}
		}
	}
	return rows
}

// elfSymbolValues maps each symbol of a linked ELF to its address.
func elfSymbolValues(path string) (map[string]uint32, error) {
	file, err := elf.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	symbols, err := file.Symbols()
	if err != nil {
		return nil, err
	}
	values := make(map[string]uint32, len(symbols))
	for _, symbol := range symbols {
		values[symbol.Name] = uint32(symbol.Value)
	}
	return values, nil
}

// relocCheckModule links m, classifies its data words, links it again at
// each shift and compares.
func (p project) relocCheckModule(resolver symbolResolver, names relocNames, m *moduleSpec) (relocReport, error) {
	original, err := p.readModuleImage(m)
	if err != nil {
		return relocReport{}, err
	}
	layout, err := moduleLinkLayout(m)
	if err != nil {
		return relocReport{}, err
	}
	report, err := relocClassify(resolver, names, m, layout, original)
	if err != nil {
		return report, err
	}
	linked, err := p.linkModule(resolver, m)
	if err != nil {
		return report, err
	}
	base, err := elfSymbolValues(linked.elf)
	if err != nil {
		return report, err
	}
	rows := make(map[uint32]string)
	for _, symbol := range linked.layout.absolute {
		if previous, ok := rows[symbol.Addr]; !ok || symbol.Name < previous {
			rows[symbol.Addr] = symbol.Name
		}
	}
	foreign := make(map[uint32]string, len(linked.bindings))
	for name, addr := range linked.bindings {
		if previous, ok := foreign[addr]; !ok || name < previous {
			foreign[addr] = name
		}
	}
	dir := filepath.Join(p.root, "build", "reloc-check", m.ID)
	if err := os.MkdirAll(dir, 0o755); err != nil {
		return report, err
	}
	for _, shift := range relocCheckShifts {
		script := filepath.Join(dir, fmt.Sprintf("shift_%x.ld", shift))
		elfPath := filepath.Join(dir, fmt.Sprintf("shift_%x.elf", shift))
		image := filepath.Join(dir, fmt.Sprintf("shift_%x.bin", shift))
		if err := atomicWrite(script, []byte(linkScript(linked.layout, linked.inputs[0], linked.bindings, linked.gp, shift))); err != nil {
			return report, err
		}
		if err := linkObjects(script, elfPath, linked.inputs); err != nil {
			return report, fmt.Errorf("%s shifted by 0x%x: %w", m.ID, shift, err)
		}
		if err := runInDir(p.root, "mipsel-linux-gnu-objcopy", "-O", "binary", elfPath, image); err != nil {
			return report, err
		}
		shifted, err := os.ReadFile(image)
		if err != nil {
			return report, err
		}
		if len(shifted) != len(original) {
			return report, fmt.Errorf("%s shifted by 0x%x has %d bytes, the file %d", m.ID, shift, len(shifted), len(original))
		}
		symbols, err := elfSymbolValues(elfPath)
		if err != nil {
			return report, err
		}
		moved := func(name string) (uint32, bool) {
			before, known := base[name]
			after, placed := symbols[name]
			return after - before, known && placed
		}
		unexplained, code := relocCompareShifted(m, linked.layout, original, shifted, shift, moved, rows, foreign)
		if len(unexplained) > 0 {
			return report, fmt.Errorf("%s shifted by 0x%x: %d words changed unexplained: %s", m.ID, shift, len(unexplained), strings.Join(unexplained[:min(len(unexplained), 3)], "; "))
		}
		for index := range code {
			code[index].what += " (" + names.describe(m, code[index].value) + ")"
		}
		report.findings = append(report.findings, code...)
	}
	report.findings = append(report.findings, relocRows(m, linked.layout)...)
	return report, nil
}

// relocPendingError fails a module whose data words in scope are not all
// relocations or values, or that holds a word like a sector table's that is
// neither a disc word nor a value, naming the first few.
func relocPendingError(report relocReport) error {
	if report.pending == 0 && report.discPending == 0 {
		return nil
	}
	var parts, first []string
	if report.pending > 0 {
		parts = append(parts, fmt.Sprintf("%d words neither relocation nor value (%d orphans)", report.pending, report.orphans))
	}
	if report.discPending > 0 {
		parts = append(parts, fmt.Sprintf("%d words like a disc sector table's neither disc word nor value", report.discPending))
	}
	for _, finding := range report.findings {
		if finding.kind == "pending" || finding.kind == "orphan" || finding.kind == "sector" {
			first = append(first, fmt.Sprintf("0x%08x = 0x%08x (%s)", finding.addr, finding.value, finding.what))
			if len(first) == 3 {
				break
			}
		}
	}
	return fmt.Errorf("%s: %s", strings.Join(parts, ", "), strings.Join(first, "; "))
}

// writeRelocList writes a module's findings as a tab-separated list.
func writeRelocList(path string, findings []relocFinding) error {
	var out strings.Builder
	out.WriteString("kind\taddr\tvalue\twhat\n")
	for _, finding := range findings {
		value := ""
		if finding.kind != "row" {
			value = fmt.Sprintf("0x%08x", finding.value)
		}
		fmt.Fprintf(&out, "%s\t0x%08x\t%s\t%s\n", finding.kind, finding.addr, value, finding.what)
	}
	return atomicWrite(path, []byte(out.String()))
}

func (p project) relocCheckCommand(args []string) error {
	flags := flag.NewFlagSet("reloc-check", flag.ContinueOnError)
	selection := flags.String("module", "", "modules to check (default: all)")
	if err := flags.Parse(args); err != nil {
		return err
	}
	if flags.NArg() != 0 {
		return errors.New("usage: tools reloc-check [--module=M]")
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
	if p, err = p.withInputMirror(); err != nil {
		return err
	}
	resolver := newSymbolResolver(config)
	names := newRelocNames(config)
	var total relocReport
	checked, failed := 0, 0
	kinds := map[string]int{}
	for _, m := range modules {
		report, err := p.relocCheckModule(resolver, names, m)
		if errors.Is(err, errNotExtracted) {
			return err
		}
		if err != nil {
			failed++
			fmt.Fprintf(p.stdout(), "FAIL %s: %s\n", m.ID, strings.ReplaceAll(firstLines(err.Error(), 8), "\n", "\n     "))
			continue
		}
		if err := writeRelocList(filepath.Join(p.root, "build", "reloc-check", m.ID+".tsv"), report.findings); err != nil {
			return err
		}
		checked++
		total.words += report.words
		total.relocations += report.relocations
		total.values += report.values
		total.pending += report.pending
		total.orphans += report.orphans
		total.disc += report.disc
		total.discPending += report.discPending
		moduleKinds := map[string]int{}
		for _, finding := range report.findings {
			moduleKinds[finding.kind]++
			kinds[finding.kind]++
		}
		if err := relocPendingError(report); err != nil {
			failed++
			fmt.Fprintf(p.stdout(), "FAIL %s: %s\n", m.ID, err)
			continue
		}
		line := fmt.Sprintf("ok   %s: %d words, %d relocations, %d values, %d pending (%d orphans)", m.ID, report.words, report.relocations, report.values, report.pending, report.orphans)
		if moduleKinds["code"]+moduleKinds["link"]+moduleKinds["row"] > 0 {
			line += fmt.Sprintf("; code: %d own addresses in place, %d through linked names, %d rows inside code", moduleKinds["code"], moduleKinds["link"], moduleKinds["row"])
		}
		fmt.Fprintln(p.stdout(), line)
	}
	var shifts []string
	for _, shift := range relocCheckShifts {
		shifts = append(shifts, fmt.Sprintf("+0x%x", shift))
	}
	fmt.Fprintf(p.stdout(), "reloc-check: %d modules, %d words to classify: %d relocations, %d values, %d pending (%d orphans if moved)\n",
		checked, total.words, total.relocations, total.values, total.pending, total.orphans)
	fmt.Fprintf(p.stdout(), "  shifts %s: %d linked with every change explained\n", strings.Join(shifts, " and "), checked)
	fmt.Fprintf(p.stdout(), "  code: %d own addresses left in place, %d more through a linked module's names, %d data rows bound inside code\n", kinds["code"], kinds["link"], kinds["row"])
	fmt.Fprintf(p.stdout(), "  disc: %d words written from the layout, %d more like a sector table's left\n", total.disc, total.discPending)
	fmt.Fprintln(p.stdout(), "  lists: build/reloc-check/<module>.tsv")
	if failed > 0 {
		return fmt.Errorf("reloc-check: %d modules failed", failed)
	}
	return nil
}
