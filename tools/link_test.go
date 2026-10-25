package main

import (
	"crypto/sha256"
	"encoding/hex"
	"os"
	"path/filepath"
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
	if len(layout.absolute) != 2 || layout.absolute[0].Name != "g_test_inside" || layout.absolute[1].Name != "g_test_past" {
		t.Fatalf("absolute %v", layout.absolute)
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
	objects := map[string]string{"test_first": "/w/first.o", "test_second": "/w/second.o"}
	script := linkScript(layout, objects, "/w/data.o", map[string]uint32{"g_test_inside": 0x8010000c, "battle_x": 0x80070000})
	for _, line := range []string{
		"PROVIDE(battle_x = 0x80070000);\nPROVIDE(g_test_inside = 0x8010000c);\n",
		"  .text_80100008 0x80100008 : SUBALIGN(4) { \"/w/first.o\"(.text) }\n",
		"  .rodata_80100020 0x80100020 : SUBALIGN(4) { \"/w/second.o\"(.rodata .rdata) }\n",
		"  .bytes_80100018 0x80100018 : SUBALIGN(1) { \"/w/data.o\"(.bytes_80100018) }\n",
		"ASSERT(test_second == 0x80100010, \"test_second must begin at 0x80100010\");\n",
		"ASSERT(g_test_tail == 0x80100030, \"g_test_tail must be at 0x80100030\");\n",
	} {
		if !strings.Contains(script, line) {
			t.Fatalf("script lacks %q:\n%s", line, script)
		}
	}
}

func TestLinkUnsupportedNamesTheBlocker(t *testing.T) {
	if reason := linkUnsupported(linkTestModule()); reason != "" {
		t.Fatalf("plain module: %q", reason)
	}
	cases := map[string]func(m *moduleSpec){
		"per-function overrides": func(m *moduleSpec) {
			m.Overrides = []overrideSpec{{Function: "test_first", Name: "g", Addr: 0x80100000}}
		},
		"by source_symbol": func(m *moduleSpec) { m.Functions[1].SourceSymbol = "shared" },
		"hand-written":     func(m *moduleSpec) { m.Functions[0].Asm = "replaces $sp" },
		"several entries": func(m *moduleSpec) {
			m.Functions[1].Source = "src/event/test_first.c"
		},
		"small data": func(m *moduleSpec) { m.Functions[0].Profile = "gcc-2.6.3_O2_G8_aspsx-2.34" },
	}
	for want, change := range cases {
		m := linkTestModule()
		change(m)
		if reason := linkUnsupported(m); !strings.Contains(reason, want) {
			t.Fatalf("%s: got %q", want, reason)
		}
	}
}

const testLinkMainYAML = `module: main
file: SCUS_942.21
lba: 24
size: 4096
sha256: 00000000000000000000000000000000000000000000000000000000000000aa
load: 0x8000f800
source_dir: src/main
profile: gcc-2.6.3_O2_aspsx-2.34

functions:
  - {addr: 0x80010000, size: 16, name: main_first, hash: 0123456789abcdef}
  - {addr: 0x80010100, size: 32, name: main_second, hash: 123456789012345a}

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
		"target/main.yaml":       testLinkMainYAML,
		"src/main/main_first.c":  "#include \"fft/main.h\"\nint main_first(void) { return g_main_value; }\n",
		"src/main/main_second.c": "#include \"fft/main.h\"\nint main_second(void) { return main_first() + 1; }\n",
		"include/fft/main.h":     "extern int g_main_value;\nint main_first(void);\n",
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
	if err := os.Remove(path); err != nil {
		t.Fatal(err)
	}
	if result, err = p.linkModule(resolver, m); err != nil || result.identical || !strings.Contains(result.checked, "2 function hashes") {
		t.Fatalf("without the disc: %+v, %v", result, err)
	}
	m.Functions[1].Hash = "0000000000000000"
	if _, err := p.linkModule(resolver, m); err == nil || !strings.Contains(err.Error(), "main_second differs") {
		t.Fatalf("wrong hash: %v", err)
	}
}
