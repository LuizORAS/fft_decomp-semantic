package main

import (
	"crypto/sha256"
	"encoding/binary"
	"encoding/hex"
	"os"
	"path/filepath"
	"reflect"
	"strings"
	"testing"
)

func linkTestModule() *moduleSpec {
	return &moduleSpec{
		ID: "event-test", File: "EVENT/TEST.OUT", Load: 0x80100000, Size: 0x40, SourceDir: "src/event",
		Profile: "gcc-2.6.3_O2_aspsx-2.34",
		Functions: []functionSpec{
			{Addr: 0x80100008, Size: 8, Name: "test_first"},
			{Addr: 0x80100010, Size: 8, Name: "test_second", Rodata: []rodataSpec{{Addr: 0x80100020, Size: 4}}},
		},
		Data: []symbolSpec{
			{Addr: 0x80100004, Name: "g_test_b"}, {Addr: 0x80100000, Name: "g_test_a"},
			{Addr: 0x8010000c, Name: "g_test_inside"}, {Addr: 0x80100030, Name: "g_test_tail"},
			{Addr: 0x80100100, Name: "g_test_past"},
		},
	}
}

func TestModuleLinkLayoutSplitsTheFile(t *testing.T) {
	layout, err := moduleLinkLayout(linkTestModule())
	if err != nil {
		t.Fatal(err)
	}
	var got []string
	for _, piece := range layout.pieces {
		entry := linkSectionName(piece)
		for _, label := range piece.labels {
			entry += " " + label.Name
		}
		got = append(got, entry)
	}
	want := []string{".bytes_80100000 g_test_a g_test_b", ".text_80100008", ".text_80100010", ".bytes_80100018",
		".rodata_80100020", ".bytes_80100024 g_test_tail"}
	if strings.Join(got, "|") != strings.Join(want, "|") {
		t.Fatalf("pieces %q", got)
	}
	// A row inside the module's own code is an alias of its piece; one past the
	// file, or one naming another module's object, stays absolute.
	if aliases := layout.pieces[1].aliases; len(aliases) != 1 || aliases[0].Name != "g_test_inside" {
		t.Fatalf("aliases of %s: %v", linkSectionName(layout.pieces[1]), aliases)
	}
	if len(layout.absolute) != 1 || layout.absolute[0].Name != "g_test_past" {
		t.Fatalf("absolute %v", layout.absolute)
	}
	foreign := linkTestModule()
	foreign.Data = append(foreign.Data, symbolSpec{Addr: 0x80100014, Name: "g_battle_inside"})
	if layout, err := moduleLinkLayout(foreign); err != nil || len(layout.absolute) != 2 || layout.absolute[0].Name != "g_battle_inside" {
		t.Fatalf("foreign row: %v %v", err, layout.absolute)
	}
	m := linkTestModule()
	m.Functions[1].Addr = 0x8010000c
	if _, err := moduleLinkLayout(m); err == nil || !strings.Contains(err.Error(), "overlaps") {
		t.Fatalf("overlap: %v", err)
	}
}

func TestLinkDataAssemblyAndScript(t *testing.T) {
	m := linkTestModule()
	layout, err := moduleLinkLayout(m)
	if err != nil {
		t.Fatal(err)
	}
	data := linkDataAssembly(m, layout, "/disc/EVENT/TEST.OUT")
	for _, line := range []string{
		"\t.section .bytes_80100000, \"a\", @progbits\n\t.globl g_test_a\ng_test_a:\n\t.incbin \"/disc/EVENT/TEST.OUT\", 0, 4\n\t.globl g_test_b\ng_test_b:\n\t.incbin \"/disc/EVENT/TEST.OUT\", 4, 4\n",
		"\t.section .bytes_80100024, \"a\", @progbits\n\t.incbin \"/disc/EVENT/TEST.OUT\", 36, 12\n\t.globl g_test_tail\ng_test_tail:\n\t.incbin \"/disc/EVENT/TEST.OUT\", 48, 16\n",
	} {
		if !strings.Contains(data, line) {
			t.Fatalf("data assembly lacks %q:\n%s", line, data)
		}
	}
	if zeroed := linkDataAssembly(m, layout, ""); !strings.Contains(zeroed, "\t.space 8\n") || strings.Contains(zeroed, "incbin") {
		t.Fatalf("zeroed data assembly:\n%s", zeroed)
	}
	bindings := map[string]uint32{"g_test_past": 0x80100100, "battle_x": 0x80070000}
	script := linkScript(layout, "/w/data.o", bindings, "", 0)
	for _, line := range []string{
		"PROVIDE(battle_x = 0x80070000);\nPROVIDE(g_test_past = 0x80100100);\n",
		"g_test_inside = ADDR(.text_80100008) + 0x4;\n",
		"ASSERT(g_test_inside == 0x8010000c, \"g_test_inside must be at 0x8010000c\");\n",
		"  .text_80100008 0x80100008 : SUBALIGN(4) { *(.text_80100008) }\n",
		"  .rodata_80100020 0x80100020 : SUBALIGN(4) { *(.rodata_80100020) }\n",
		"  .bytes_80100018 0x80100018 : SUBALIGN(1) { *(.bytes_80100018) }\n",
		"  /DISCARD/ : { \"/w/data.o\"(.text) ",
		"ASSERT(test_second == 0x80100010, \"test_second must begin at 0x80100010\");\n",
		"ASSERT(g_test_tail == 0x80100030, \"g_test_tail must be at 0x80100030\");\n",
	} {
		if !strings.Contains(script, line) {
			t.Fatalf("script lacks %q:\n%s", line, script)
		}
	}
	// Shifted, every piece, label and alias moves; the bound names do not.
	shifted := linkScript(layout, "/w/data.o", bindings, "", 0x10010)
	for _, line := range []string{
		"PROVIDE(battle_x = 0x80070000);\nPROVIDE(g_test_past = 0x80100100);\n",
		"ASSERT(g_test_inside == 0x8011001c, \"g_test_inside must be at 0x8011001c\");\n",
		"  .text_80100008 0x80110018 : SUBALIGN(4) { *(.text_80100008) }\n",
		"  .bytes_80100018 0x80110028 : SUBALIGN(1) { *(.bytes_80100018) }\n",
		"ASSERT(test_second == 0x80110020, \"test_second must begin at 0x80110020\");\n",
		"ASSERT(g_test_tail == 0x80110040, \"g_test_tail must be at 0x80110040\");\n",
	} {
		if !strings.Contains(shifted, line) {
			t.Fatalf("shifted script lacks %q:\n%s", line, shifted)
		}
	}
}

func TestLinkDataAssemblyWritesRelocatedWords(t *testing.T) {
	m := linkTestModule()
	m.Relocations = []relocationSpec{{Addr: 0x80100030, To: "g_test_a", Offset: 4}, {Addr: 0x80100024, To: "test_first"}}
	layout, err := moduleLinkLayout(m)
	if err != nil {
		t.Fatal(err)
	}
	want := "\t.section .bytes_80100024, \"a\", @progbits\n\t.word test_first\n\t.incbin \"/disc/EVENT/TEST.OUT\", 40, 8\n" +
		"\t.globl g_test_tail\ng_test_tail:\n\t.word g_test_a+0x4\n\t.incbin \"/disc/EVENT/TEST.OUT\", 52, 12\n"
	if data := linkDataAssembly(m, layout, "/disc/EVENT/TEST.OUT"); !strings.Contains(data, want) {
		t.Fatalf("data assembly:\n%s", data)
	}
	if zeroed := linkDataAssembly(m, layout, ""); !strings.Contains(zeroed, "\t.word test_first\n\t.space 8\n") {
		t.Fatalf("zeroed data assembly:\n%s", zeroed)
	}
	// An array base biased into a pointer's last byte.
	m.Data = append(m.Data, symbolSpec{Addr: 0x80100027, Name: "g_test_biased"})
	if layout, err = moduleLinkLayout(m); err != nil {
		t.Fatal(err)
	}
	if data := linkDataAssembly(m, layout, ""); !strings.Contains(data, "\t.word test_first\n\t.globl g_test_biased\n\t.set g_test_biased, . - 1\n\t.space 8\n") {
		t.Fatalf("label inside a relocated word:\n%s", data)
	}
	m.Relocations = []relocationSpec{{Addr: 0x80100008, To: "test_second"}}
	if _, err := moduleLinkLayout(m); err == nil || !strings.Contains(err.Error(), "not a word of the bytes") {
		t.Fatalf("a relocation in code: %v", err)
	}
}

func TestLinkDataAssemblyWritesDiscWords(t *testing.T) {
	m := linkTestModule()
	m.Relocations = []relocationSpec{{Addr: 0x80100024, To: "test_first"}}
	m.DiscWords = []discWordSpec{
		{Addr: 0x80100028, Kind: "extent", File: "SYSTEM.CNF", values: []uint32{23, 2048}},
		{Addr: 0x80100034, Kind: "sector", File: "MAP/MAP001.GNS", Offset: 1, values: []uint32{32}},
	}
	layout, err := moduleLinkLayout(m)
	if err != nil {
		t.Fatal(err)
	}
	want := "\t.word test_first\n\t.word 23\n\t.word 2048\n\t.globl g_test_tail\ng_test_tail:\n" +
		"\t.incbin \"/disc/EVENT/TEST.OUT\", 48, 4\n\t.word 32\n\t.incbin \"/disc/EVENT/TEST.OUT\", 56, 8\n"
	if data := linkDataAssembly(m, layout, "/disc/EVENT/TEST.OUT"); !strings.Contains(data, want) {
		t.Fatalf("data assembly:\n%s", data)
	}
	m.DiscWords[0].values = nil
	if _, err := moduleLinkLayout(m); err == nil || !strings.Contains(err.Error(), "has no SYSTEM.CNF in the disc layout") {
		t.Fatalf("a disc word without the layout: %v", err)
	}
}

func TestMainLinkWritesTheEXEHeaderAndGlobalPointer(t *testing.T) {
	m := &moduleSpec{
		ID: mainModuleID, File: "SCUS_942.21", Load: 0x8000f800, Size: 0x1000, SourceDir: "src/main",
		Profile: "gcc-2.6.3_O2_aspsx-2.34",
		Functions: []functionSpec{
			{Addr: 0x80010000, Size: 8, Name: "__SN_ENTRY_POINT"},
			{Addr: 0x80010010, Size: 8, Name: "main_heap_alloc", Profile: "gcc-2.6.3_O2_G8_aspsx-2.34"},
		},
		Data: []symbolSpec{{Addr: 0x800105bc, Name: "g_saved_gp_slot"}},
	}
	layout, err := moduleLinkLayout(m)
	if err != nil {
		t.Fatal(err)
	}
	if layout.pieces[0].kind != "header" || layout.pieces[0].size != psxEXEHeaderSize || layout.pieces[1].addr != 0x80010000 {
		t.Fatalf("pieces %+v", layout.pieces[:2])
	}
	data := linkDataAssembly(m, layout, "/disc/SCUS_942.21")
	want := "\t.section .header_8000f800, \"a\", @progbits\n\t.ascii \"PS-X EXE\"\n\t.incbin \"/disc/SCUS_942.21\", 8, 8\n" +
		"\t.word __SN_ENTRY_POINT\n\t.incbin \"/disc/SCUS_942.21\", 20, 4\n\t.word __exe_text_start\n\t.word __exe_text_size\n" +
		"\t.incbin \"/disc/SCUS_942.21\", 32, 2016\n"
	if !strings.Contains(data, want) {
		t.Fatalf("header:\n%s", data)
	}
	if _, err := linkGlobalPointer(m, layout); err == nil || !strings.Contains(err.Error(), "0x800329bc") {
		t.Fatalf("no label at the global pointer: %v", err)
	}
	m.Data = append(m.Data, symbolSpec{Addr: 0x800329bc, Name: "g_psyq_crt_saved_ra"})
	m.Size = 0x24000
	if layout, err = moduleLinkLayout(m); err != nil {
		t.Fatal(err)
	}
	gp, err := linkGlobalPointer(m, layout)
	if err != nil || gp != "g_psyq_crt_saved_ra" {
		t.Fatalf("global pointer %q, %v", gp, err)
	}
	script := linkScript(layout, "/w/data.o", nil, gp, 0)
	for _, line := range []string{
		"OUTPUT_ARCH(mips)\n_gp = g_psyq_crt_saved_ra;\n",
		"__exe_text_start = ADDR(.text_80010000);\n",
		"__exe_text_size = ADDR(.bytes_80010018) + SIZEOF(.bytes_80010018) - __exe_text_start;\n",
	} {
		if !strings.Contains(script, line) {
			t.Fatalf("script lacks %q:\n%s", line, script)
		}
	}
}

func TestModuleLinkLayoutShadowsSharedLiterals(t *testing.T) {
	m := linkTestModule()
	m.Functions[0].Rodata = []rodataSpec{{Addr: 0x80100020, Size: 2}}
	m.Functions[1].Rodata = []rodataSpec{{Addr: 0x80100020, Size: 4}}
	layout, err := moduleLinkLayout(m)
	if err != nil {
		t.Fatal(err)
	}
	if len(layout.shadows) != 1 || linkShadowName(layout.shadows[0]) != ".shadow_80100020_test_first" {
		t.Fatalf("shadows %v", layout.shadows)
	}
	script := linkScript(layout, "/w/data.o", nil, "", 0)
	if !strings.Contains(script, "  .rodata_80100020 0x80100020 : SUBALIGN(4) { *(.rodata_80100020) }\n") ||
		!strings.Contains(script, "  .shadow_80100020_test_first 0x80100020 (NOLOAD) : SUBALIGN(4) { *(.shadow_80100020_test_first) }\n") {
		t.Fatalf("script:\n%s", script)
	}
	sections, err := linkObjectSections(layout)
	if err != nil {
		t.Fatal(err)
	}
	want := map[string]map[string]string{
		"test_first":  {".text": ".text_80100008", ".rodata": ".shadow_80100020_test_first", ".rdata": ".shadow_80100020_test_first"},
		"test_second": {".text": ".text_80100010", ".rodata": ".rodata_80100020", ".rdata": ".rodata_80100020"},
	}
	if !reflect.DeepEqual(sections, want) {
		t.Fatalf("object sections %v", sections)
	}
	m.Functions[0].Rodata = []rodataSpec{{Addr: 0x80100022, Size: 4}}
	if _, err := moduleLinkLayout(m); err == nil || !strings.Contains(err.Error(), "overlaps") {
		t.Fatalf("a literal that runs past the other: %v", err)
	}
}

func TestLinkObjectRenamesBindOverridesSymbolically(t *testing.T) {
	m := linkTestModule()
	m.Functions[1].Source, m.Functions[1].SourceSymbol = "src/shared/test/shared.c", "shared_entry"
	m.Overrides = []overrideSpec{
		{Function: "test_first", Name: "g_test_a", Addr: 0x80100000},       // module-wide already
		{Function: "test_first", Name: "shared_entry", Addr: 0x80100010},   // the module's name there
		{Function: "test_second", Name: "battle_helper", Addr: 0x80070000}, // a linked module's
		{Function: "test_second", Name: "g_unnamed", Addr: 0x80100038},     // inside the bytes
		{Function: "test_second", Name: "g_far", Addr: 0x80123400},         // nowhere named
	}
	m.Links = []string{"battle"}
	config := &projectConfig{Modules: []*moduleSpec{m,
		{ID: "battle", Functions: []functionSpec{{Addr: 0x80070000, Name: "battle_helper_entry"}}},
		{ID: mainModuleID}}}
	resolver := newSymbolResolver(config)
	layout, err := moduleLinkLayout(m)
	if err != nil {
		t.Fatal(err)
	}
	renames, absolute, err := linkObjectRenames(resolver, m, &layout)
	if err != nil {
		t.Fatal(err)
	}
	if got := renames["test_first"]; len(got) != 1 || got["shared_entry"] != "test_second" {
		t.Fatalf("test_first renames %v", got)
	}
	want := map[string]string{"shared_entry": "test_second", "battle_helper": "battle_helper_entry", "g_unnamed": "D_80100038", "g_far": "D_80123400"}
	if got := renames["test_second"]; len(got) != len(want) {
		t.Fatalf("test_second renames %v", got)
	}
	for old, new := range want {
		if renames["test_second"][old] != new {
			t.Fatalf("test_second renames %v", renames["test_second"])
		}
	}
	if len(absolute) != 1 || absolute["D_80123400"] != 0x80123400 {
		t.Fatalf("absolute %v", absolute)
	}
	tail := layout.pieces[len(layout.pieces)-1]
	if len(tail.labels) != 2 || tail.labels[0].Name != "g_test_tail" || tail.labels[1].Name != "D_80100038" {
		t.Fatalf("tail labels %v", tail.labels)
	}
}

const testLinkMainYAML = `module: main
file: SCUS_942.21
size: 4096
sha256: 00000000000000000000000000000000000000000000000000000000000000aa
load: 0x8000f800
source_dir: src/main
profile: gcc-2.6.3_O2_aspsx-2.34

functions:
  - {addr: 0x80010000, size: 16, name: main_first, hash: 0123456789abcdef}
  - {addr: 0x80010100, size: 32, name: main_second, hash: 123456789012345a}
  - {addr: 0x80010200, size: 8, name: __SN_ENTRY_POINT, hash: 23456789012345ab}

data:
  - {addr: 0x80010080, name: g_main_value}
  - {addr: 0x80010400, name: g_main_tail}
`

// TestLinkModuleRebuildsTheOriginalImage builds an "original" image from the
// per-function compiles and original-looking bytes, then links the module
// whole: the image must come back identical, and without it every function
// must still match its hash.
func TestLinkModuleRebuildsTheOriginalImage(t *testing.T) {
	requirePinnedToolchain(t)
	t.Setenv("TOOLS_CACHE", "0")
	t.Setenv("TOOLS_WORK_DIR", t.TempDir())
	p := testProject(t, map[string]string{
		"target/main.yaml":            testLinkMainYAML,
		"src/main/main_first.c":       "#include \"fft/main.h\"\nint main_first(void) { return g_main_value; }\n",
		"src/main/main_second.c":      "#include \"fft/main.h\"\nint main_second(void) { return main_first() + 1; }\n",
		"include/fft/main.h":          "extern int g_main_value;\nint main_first(void);\n",
		"src/main/__SN_ENTRY_POINT.c": "void __SN_ENTRY_POINT(void) {}\n",
	})
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	m, _ := config.module("main")
	resolver := newSymbolResolver(config)
	image := make([]byte, m.Size)
	for index := range image {
		image[index] = byte(index*7 + 3)
	}
	for index := range m.Functions {
		f := &m.Functions[index]
		sections, err := p.compileFunction(resolver, m, f, filepath.Join(p.root, "build", "learn"))
		if err != nil {
			t.Fatal(err)
		}
		f.Size, f.Hash = len(sections[".text"]), shortHash(sections[".text"])
		copy(image[m.offset(f.Addr):], sections[".text"])
	}
	// The PS-X EXE fields the link writes: the entry point and the text.
	copy(image, "PS-X EXE")
	binary.LittleEndian.PutUint32(image[0x10:], 0x80010200)
	binary.LittleEndian.PutUint32(image[0x18:], 0x80010000)
	binary.LittleEndian.PutUint32(image[0x1c:], uint32(m.Size-psxEXEHeaderSize))
	// Two data words hold addresses, main_second's and four bytes into
	// g_main_value; the link writes them from the symbols.
	m.Relocations = []relocationSpec{{Addr: 0x80010300, To: "main_second"}, {Addr: 0x80010400, To: "g_main_value", Offset: 4}}
	binary.LittleEndian.PutUint32(image[m.offset(0x80010300):], 0x80010100)
	binary.LittleEndian.PutUint32(image[m.offset(0x80010400):], 0x80010084)
	path := filepath.Join(p.filesRoot, m.File)
	if err := os.MkdirAll(filepath.Dir(path), 0o755); err != nil {
		t.Fatal(err)
	}
	if err := os.WriteFile(path, image, 0o644); err != nil {
		t.Fatal(err)
	}
	sum := sha256.Sum256(image)
	m.SHA256 = hex.EncodeToString(sum[:])
	result, err := p.linkModule(resolver, m)
	if err != nil {
		t.Fatal(err)
	}
	if !result.identical {
		t.Fatalf("not identical: %+v", result)
	}
	script, err := os.ReadFile(filepath.Join(p.root, "build", "link", "main", "link.ld"))
	if err != nil {
		t.Fatal(err)
	}
	if strings.Contains(string(script), "PROVIDE(main_first") || strings.Contains(string(script), "PROVIDE(g_main_value") {
		t.Fatalf("own names must come from the link, not PROVIDE:\n%s", script)
	}
	data, err := os.ReadFile(filepath.Join(p.root, "build", "link", "main", "data.s"))
	if err != nil {
		t.Fatal(err)
	}
	if !strings.Contains(string(data), "\t.word main_second\n") || !strings.Contains(string(data), "g_main_tail:\n\t.word g_main_value+0x4\n") {
		t.Fatalf("relocated words:\n%s", data)
	}
	m.Relocations[0].To = "main_first"
	if _, err := p.linkModule(resolver, m); err == nil || !strings.Contains(err.Error(), "linked image differs at file offset 0xb01") {
		t.Fatalf("a relocation to the wrong symbol: %v", err)
	}
	m.Relocations[0].To = "main_second"
	if err := os.Remove(path); err != nil {
		t.Fatal(err)
	}
	if result, err = p.linkModule(resolver, m); err != nil || result.identical || !strings.Contains(result.checked, "3 function hashes") {
		t.Fatalf("without the disc: %+v, %v", result, err)
	}
	m.Functions[1].Hash = "0000000000000000"
	if _, err := p.linkModule(resolver, m); err == nil || !strings.Contains(err.Error(), "main_second differs") {
		t.Fatalf("wrong hash: %v", err)
	}
}
