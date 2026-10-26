package main

import (
	"crypto/sha256"
	"encoding/binary"
	"encoding/hex"
	"errors"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

func TestShiftedInstructionExplained(t *testing.T) {
	for _, c := range []struct {
		before, after, shift uint32
		want                 bool
	}{
		{0x0c040004, 0x0c040008, 0x10, true},    // jal: the target field counts words
		{0x0c040004, 0x0c044008, 0x10010, true}, // jal past 64 KB
		{0x0c040004, 0x0c04000c, 0x10, false},   // jal moved too far
		{0x3c028010, 0x3c028011, 0x10, true},    // lui: the carry of the low half
		{0x3c028010, 0x3c028011, 0x10010, true}, // lui: the high half
		{0x3c028010, 0x3c028012, 0x10010, true}, // lui: both
		{0x3c028010, 0x3c028012, 0x10, false},   // lui moved too far
		{0x8c420080, 0x8c420090, 0x10, true},    // lw: the low half
		{0x8c420080, 0x8c420090, 0x10010, true}, // lw past 64 KB
		{0x8c420080, 0x8c430090, 0x10, false},   // another register
		{0x8c420080, 0xac420090, 0x10, false},   // another instruction
		{0x24420080, 0x24420081, 0x10, false},   // addiu moved by a byte
		{0x00000000, 0x00000010, 0x10, false},   // a nop is no relocation
	} {
		if got := shiftedInstructionExplained(c.before, c.after, c.shift); got != c.want {
			t.Errorf("0x%08x -> 0x%08x by 0x%x: %v, want %v", c.before, c.after, c.shift, got, c.want)
		}
	}
}

func TestPairedAddressCompletesALui(t *testing.T) {
	image := make([]byte, 0x20)
	put := func(offset int, word uint32) { binary.LittleEndian.PutUint32(image[offset:], word) }
	put(0x00, 0x3c028010) // lui v0, 0x8010
	put(0x08, 0x8c448010) // lw a0, -0x7ff0(v0)
	put(0x10, 0x3c018010) // lui at, 0x8010
	put(0x14, 0x34211234) // ori at, at, 0x1234
	put(0x18, 0x3c038010) // lui v1, 0x8010, never completed
	if addr, ok := pairedAddress(image, 0x00, len(image)); !ok || addr != 0x800f8010 {
		t.Fatalf("lw: 0x%08x %v", addr, ok)
	}
	if addr, ok := pairedAddress(image, 0x10, len(image)); !ok || addr != 0x80101234 {
		t.Fatalf("ori: 0x%08x %v", addr, ok)
	}
	if _, ok := pairedAddress(image, 0x18, len(image)); ok {
		t.Fatal("a lui with no use paired")
	}
}

func TestRelocNamesDescribeValues(t *testing.T) {
	m := linkTestModule()
	m.Links = []string{"battle"}
	config := &projectConfig{Modules: []*moduleSpec{m,
		{ID: "battle", Load: 0x80070000, Size: 0x100,
			Functions: []functionSpec{{Addr: 0x80070000, Size: 8, Name: "battle_x"}}, Data: []symbolSpec{{Addr: 0x80070040, Name: "g_battle_y"}}},
		{ID: "effect-e000", Load: 0x80180000, Size: 0x100},
		{ID: "effect-e001", Load: 0x80180000, Size: 0x80},
		{ID: mainModuleID, Load: 0x8000f800, Size: 0x1000, Data: []symbolSpec{{Addr: 0x80010080, Name: "g_main_value"}}},
	}}
	names := newRelocNames(config)
	for value, want := range map[uint32]string{
		0x80100008: "function test_first",
		0x8010000a: "inside test_first+0x2",
		0x80100004: "data g_test_b",
		0x80100034: "past g_test_tail+0x4",
		0x80070040: "battle data g_battle_y",
		0x80070044: "battle past g_battle_y+0x4",
		0x80010080: "main data g_main_value",
		0x80180090: "in effect-e000's file",
		0x80180010: "in 2 modules' files (effect-e000, ...)",
		0x80150000: "nothing",
	} {
		if got := names.describe(m, value); got != want {
			t.Errorf("0x%08x: %q, want %q", value, got, want)
		}
	}
}

func TestRelocPendingErrorFailsAModuleWithPendingWords(t *testing.T) {
	report := relocReport{words: 3, relocations: 2, values: 1,
		findings: []relocFinding{{kind: "code", addr: 0x80100000, value: 0x80100040, what: "lui"}}}
	if err := relocPendingError(report); err != nil {
		t.Fatalf("a fully classified module failed: %v", err)
	}
	report.pending, report.orphans = 4, 1
	report.findings = append(report.findings,
		relocFinding{kind: "pending", addr: 0x80100010, value: 0x80010080, what: "main data g_main_value"},
		relocFinding{kind: "orphan", addr: 0x80100014, value: 0x80100004, what: "data g_test_b"},
		relocFinding{kind: "pending", addr: 0x80100018, value: 0x80070044, what: "battle past g_battle_y+0x4"},
		relocFinding{kind: "pending", addr: 0x8010001c, value: 0x80150000, what: "nothing"})
	err := relocPendingError(report)
	if err == nil {
		t.Fatal("a module with pending words passed")
	}
	for _, want := range []string{"4 words neither relocation nor value (1 orphans)", "0x80100010 = 0x80010080 (main data g_main_value)", "0x80100018"} {
		if !strings.Contains(err.Error(), want) {
			t.Errorf("%q lacks %q", err, want)
		}
	}
	for _, unwanted := range []string{"0x8010001c", "lui"} {
		if strings.Contains(err.Error(), unwanted) {
			t.Errorf("%q names %q: only the first three pending words belong there", err, unwanted)
		}
	}
}

func TestRelocCompareShiftedExplainsEachChange(t *testing.T) {
	m := linkTestModule()
	m.Relocations = []relocationSpec{{Addr: 0x80100024, To: "test_first"}}
	layout, err := moduleLinkLayout(m)
	if err != nil {
		t.Fatal(err)
	}
	put := func(image []byte, addr, word uint32) { binary.LittleEndian.PutUint32(image[m.offset(addr):], word) }
	original := make([]byte, m.Size)
	put(original, 0x80100008, 0x0c040004) // jal test_second
	put(original, 0x80100010, 0x3c028010) // lui v0, %hi(g_test_a)
	put(original, 0x80100014, 0x8c420000) // lw v0, %lo(g_test_a)(v0)
	put(original, 0x80100020, 0x80100008) // .rodata: test_first
	put(original, 0x80100024, 0x80100008) // relocated: test_first
	shifted := func(shift uint32, jal, lui uint32) []byte {
		image := append([]byte(nil), original...)
		put(image, 0x80100008, jal)
		put(image, 0x80100010, lui)
		put(image, 0x80100014, 0x8c420000+shift&0xffff)
		put(image, 0x80100020, 0x80100008+shift)
		put(image, 0x80100024, 0x80100008+shift)
		return image
	}
	moved := func(shift uint32) func(string) (uint32, bool) {
		return func(name string) (uint32, bool) { return shift, name == "test_first" }
	}
	image := shifted(0x10, 0x0c040008, 0x3c028010)
	if unexplained, code := relocCompareShifted(m, layout, original, image, 0x10, moved(0x10), nil, nil); len(unexplained)+len(code) != 0 {
		t.Fatalf("shifted by 0x10: %v %v", unexplained, code)
	}
	put(image, 0x80100000, 0x10)         // a data word that is no relocation
	put(image, 0x80100024, 0x80100028)   // a relocation moved too far
	put(image, 0x80100020, 0x8010000c+4) // a .rodata word moved too far
	unexplained, _ := relocCompareShifted(m, layout, original, image, 0x10, moved(0x10), nil, nil)
	if len(unexplained) != 3 || !strings.HasPrefix(unexplained[0], "0x80100000 bytes 0x00000000 -> 0x00000010") ||
		!strings.HasPrefix(unexplained[1], "0x80100020 rodata") || !strings.HasPrefix(unexplained[2], "0x80100024 bytes") {
		t.Fatalf("unexplained %q", unexplained)
	}
	// Past 64 KB every high half into the module moves: one left in place
	// names an own address, a number in the source or a data row bound inside
	// code, or a linked module's name bound there.
	image = shifted(0x10010, 0x0c044008, 0x3c028010)
	unexplained, code := relocCompareShifted(m, layout, original, image, 0x10010, moved(0x10010), nil, nil)
	if len(unexplained) != 0 || len(code) != 1 || code[0].kind != "code" || code[0].addr != 0x80100010 || code[0].value != 0x80100000 || code[0].what != "test_second+0x0" {
		t.Fatalf("fixed lui: %v %+v", unexplained, code)
	}
	foreign := map[uint32]string{0x80100000: "battle_y"}
	if _, code = relocCompareShifted(m, layout, original, image, 0x10010, moved(0x10010), map[uint32]string{0x80100000: "g_test_row"}, foreign); len(code) != 1 || code[0].kind != "code" || code[0].what != "test_second+0x0, bound to g_test_row" {
		t.Fatalf("fixed lui on a row: %+v", code)
	}
	if _, code = relocCompareShifted(m, layout, original, image, 0x10010, moved(0x10010), nil, foreign); len(code) != 1 || code[0].kind != "link" || code[0].what != "test_second+0x0, bound to battle_y" {
		t.Fatalf("a linked module's name: %+v", code)
	}
}

// TestRelocCheckModuleClassifiesAndShifts links a small MAIN whose data hold
// two relocated words, an own address left unclassified, a declared value, a
// word naming nothing and a kernel word, and whose code reads an own address
// written as a number; then links it shifted.
func TestRelocCheckModuleClassifiesAndShifts(t *testing.T) {
	requirePinnedToolchain(t)
	t.Setenv("TOOLS_CACHE", "0")
	t.Setenv("TOOLS_WORK_DIR", t.TempDir())
	p := testProject(t, map[string]string{
		"target/main.yaml":            testLinkMainYAML,
		"src/main/main_first.c":       "#include \"fft/main.h\"\nint main_first(void) { return g_main_value; }\n",
		"src/main/main_second.c":      "#include \"fft/main.h\"\nint main_second(void) { return main_first() + *(volatile int *)0x80010084; }\n",
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
	for index := range m.Functions {
		f := &m.Functions[index]
		sections, err := p.compileFunction(resolver, m, f, filepath.Join(p.root, "build", "learn"))
		if err != nil {
			t.Fatal(err)
		}
		f.Size, f.Hash = len(sections[".text"]), shortHash(sections[".text"])
		copy(image[m.offset(f.Addr):], sections[".text"])
	}
	copy(image, "PS-X EXE")
	binary.LittleEndian.PutUint32(image[0x10:], 0x80010200)
	binary.LittleEndian.PutUint32(image[0x18:], 0x80010000)
	binary.LittleEndian.PutUint32(image[0x1c:], uint32(m.Size-psxEXEHeaderSize))
	for addr, word := range map[uint32]uint32{
		0x80010300: 0x80010100, // relocated: main_second
		0x80010304: 0x80010100, // main_second's address, unclassified
		0x80010308: 0x80010000, // a declared value
		0x8001030c: 0x80150000, // names nothing
		0x80010310: 0x80008000, // two s16 -32768, below the scope
		0x80010400: 0x80010084, // relocated: g_main_value+4
	} {
		binary.LittleEndian.PutUint32(image[m.offset(addr):], word)
	}
	m.Relocations = []relocationSpec{{Addr: 0x80010300, To: "main_second"}, {Addr: 0x80010400, To: "g_main_value", Offset: 4}}
	m.Values = []valueSpec{{Addr: 0x80010308, Why: "test"}}
	m.Data = append(m.Data, symbolSpec{Addr: 0x80010004, Name: "g_main_inside"})
	path := filepath.Join(p.filesRoot, m.File)
	if err := os.MkdirAll(filepath.Dir(path), 0o755); err != nil {
		t.Fatal(err)
	}
	if err := os.WriteFile(path, image, 0o644); err != nil {
		t.Fatal(err)
	}
	sum := sha256.Sum256(image)
	m.SHA256 = hex.EncodeToString(sum[:])
	names := newRelocNames(config)
	report, err := p.relocCheckModule(resolver, names, m)
	if err != nil {
		t.Fatal(err)
	}
	if report.words != 5 || report.relocations != 2 || report.values != 1 || report.pending != 2 || report.orphans != 1 {
		t.Fatalf("report %+v", report)
	}
	var got []string
	for _, finding := range report.findings {
		got = append(got, finding.kind+" "+finding.what)
	}
	want := []string{"orphan function main_second", "pending nothing"}
	if len(got) != 4 || got[0] != want[0] || got[1] != want[1] || !strings.HasPrefix(got[2], "code main_second+0x") ||
		report.findings[2].value != 0x80010084 || got[3] != "row g_main_inside in main_first .text+0x4" {
		t.Fatalf("findings %q", got)
	}
	if err := writeRelocList(filepath.Join(p.root, "build", "reloc-check", "main.tsv"), report.findings); err != nil {
		t.Fatal(err)
	}
	list, err := os.ReadFile(filepath.Join(p.root, "build", "reloc-check", "main.tsv"))
	if err != nil || !strings.HasPrefix(string(list), "kind\taddr\tvalue\twhat\norphan\t0x80010304\t0x80010100\tfunction main_second\n") ||
		!strings.Contains(string(list), "row\t0x80010004\t\tg_main_inside") {
		t.Fatalf("list:\n%s", list)
	}
	m.Relocations[0].To = "main_first"
	if _, err := p.relocCheckModule(resolver, names, m); err == nil || !strings.Contains(err.Error(), "relocation 0x80010300 holds 0x80010100, but main_first+0x0 is 0x80010000") {
		t.Fatalf("a relocation to the wrong symbol: %v", err)
	}
	if err := os.Remove(path); err != nil {
		t.Fatal(err)
	}
	if _, err := p.relocCheckModule(resolver, names, m); !errors.Is(err, errNotExtracted) {
		t.Fatalf("without the disc: %v", err)
	}
}
