package main

import (
	"path/filepath"
	"strings"
	"testing"
)

const testUnitMainYAML = `module: main
file: SCUS_942.21
lba: 24
size: 4096
sha256: 00000000000000000000000000000000000000000000000000000000000000aa
load: 0x8000f800
source_dir: src/main
profile: gcc-2.6.3_O2_aspsx-2.34

units:
  - {addr: 0x80010000, end: 0x80010018, name: pair, profile: gcc-2.6.3_O1_aspsx-2.34}

functions:
  - {addr: 0x80010000, size: 8, name: main_first, hash: 0123456789abcdef}
  - {addr: 0x80010008, size: 16, name: main_second, hash: 123456789012345a}
  - {addr: 0x80010018, size: 16, name: main_third, hash: 23456789012345ab, source: src/main/main_second.c, source_symbol: main_second}

data:
  - {addr: 0x80010100, name: g_main_value}
  - {addr: 0x80010104, name: g_main_other}
`

func unitTestProject(t *testing.T, mainYAML string) project {
	t.Helper()
	return testProject(t, map[string]string{
		"target/main.yaml":       mainYAML,
		"src/main/main_first.c":  "#include \"fft/main.h\"\nvoid main_first(void) {}\n",
		"src/main/main_second.c": "#include \"fft/main.h\"\n/* the body */\nint main_second(void) { return g_main_value; }\n",
		"include/fft/main.h":     "extern int g_main_value;\nextern int g_main_other;\n",
	})
}

func TestUnitRowsRoundTripAndValidate(t *testing.T) {
	p := unitTestProject(t, testUnitMainYAML)
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	m, _ := config.module("main")
	if len(m.Units) != 1 || m.Units[0].Name != "pair" || len(m.unitMembers(m.Units[0])) != 2 || m.unitProfileName(m.Units[0]) != "gcc-2.6.3_O1_aspsx-2.34" {
		t.Fatalf("units = %+v", m.Units)
	}
	if string(renderModuleConfig(m)) != testUnitMainYAML {
		t.Fatalf("not canonical:\n%s", renderModuleConfig(m))
	}
	if err := p.validateProjectConfig(config); err != nil {
		t.Fatal(err)
	}
	for name, row := range map[string]string{
		"gap":       "{addr: 0x80010000, end: 0x80010014, name: pair}",
		"start":     "{addr: 0x80010004, end: 0x80010018, name: pair}",
		"outside":   "{addr: 0x80010000, end: 0x80020000, name: pair}",
		"profile":   "{addr: 0x80010000, end: 0x80010018, name: pair, profile: gcc-9}",
		"bad name":  "{addr: 0x80010000, end: 0x80010010, name: \"a pair\"}",
		"duplicate": "{addr: 0x80010000, end: 0x80010008, name: pair}\n  - {addr: 0x80010008, end: 0x80010018, name: pair}",
	} {
		broken := unitTestProject(t, strings.Replace(testUnitMainYAML, "{addr: 0x80010000, end: 0x80010018, name: pair, profile: gcc-2.6.3_O1_aspsx-2.34}", row, 1))
		config, err := broken.loadProjectConfig()
		if err != nil {
			t.Fatal(err)
		}
		if err := broken.validateProjectConfig(config); err == nil || !strings.Contains(err.Error(), "unit") {
			t.Errorf("%s: %v", name, err)
		}
	}
}

func TestMergeUnitSourcesHoistsIncludesAndRenamesDefinitions(t *testing.T) {
	p := unitTestProject(t, strings.Replace(testUnitMainYAML, "end: 0x80010018, name: pair, profile: gcc-2.6.3_O1_aspsx-2.34", "end: 0x80010028, name: pair", 1))
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	m, _ := config.module("main")
	build, err := resolveUnitBuild(m, m.Units[0])
	if err != nil {
		t.Fatal(err)
	}
	if build.renames, err = p.unitRenames(newSymbolResolver(config), build); err != nil {
		t.Fatal(err)
	}
	merged, err := mergeUnitSources(p.root, build, nil)
	if err != nil {
		t.Fatal(err)
	}
	want := "#include \"fft/main.h\"\n\nvoid main_first(void) {}\n\n/* the body */\nint main_second(void) { return g_main_value; }\n\n" +
		"/* the body */\nint main_third(void) { return g_main_value; }\n"
	if string(merged) != want {
		t.Fatalf("merged:\n%s", merged)
	}
}

// An override that another member contradicts takes the name of the symbol
// at its address; one that no other member contradicts stays a binding.
func TestUnitRenamesOnlyNamesOneObjectCannotShare(t *testing.T) {
	yaml := strings.Replace(testUnitMainYAML, "end: 0x80010018, name: pair, profile: gcc-2.6.3_O1_aspsx-2.34", "end: 0x80010028, name: pair", 1) +
		"\noverrides:\n  - {function: main_third, name: g_main_value, addr: 0x80010104}\n"
	p := unitTestProject(t, yaml)
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	m, _ := config.module("main")
	resolver := newSymbolResolver(config)
	build, err := resolveUnitBuild(m, m.Units[0])
	if err != nil {
		t.Fatal(err)
	}
	if build.renames, err = p.unitRenames(resolver, build); err != nil {
		t.Fatal(err)
	}
	if got := build.renames["main_third"]; got["g_main_value"] != "g_main_other" || got["main_second"] != "main_third" {
		t.Fatalf("main_third renames = %v", got)
	}
	if len(build.renames["main_second"]) != 0 {
		t.Fatalf("main_second renames = %v", build.renames["main_second"])
	}
	bindings, err := unitBindings(resolver, build, map[string]struct{}{"g_main_value": {}, "g_main_other": {}})
	if err != nil || bindings["g_main_value"] != 0x80010100 || bindings["g_main_other"] != 0x80010104 {
		t.Fatalf("bindings = %v, %v", bindings, err)
	}
	// Alone in its unit, main_third keeps the override as a binding.
	m.Units[0] = unitSpec{Addr: 0x80010018, End: 0x80010028, Name: "third"}
	alone, err := resolveUnitBuild(m, m.Units[0])
	if err != nil {
		t.Fatal(err)
	}
	if alone.renames, err = p.unitRenames(resolver, alone); err != nil {
		t.Fatal(err)
	}
	if _, renamed := alone.renames["main_third"]["g_main_value"]; renamed {
		t.Fatalf("lone renames = %v", alone.renames["main_third"])
	}
}

// The unit compiles whole: every member matches the hash it has compiled
// alone, and a member that compiles differently in the unit is named.
func TestUnitCheckCompilesTheUnitWhole(t *testing.T) {
	requirePinnedToolchain(t)
	p := unitTestProject(t, strings.Replace(testUnitMainYAML, ", profile: gcc-2.6.3_O1_aspsx-2.34", "", 1))
	t.Setenv("TOOLS_CACHE", "0")
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	m, _ := config.module("main")
	resolver := newSymbolResolver(config)
	for index := range m.Functions[:2] {
		f := &m.Functions[index]
		sections, err := p.compileFunction(resolver, m, f, filepath.Join(p.root, "build", "learn"))
		if err != nil {
			t.Fatal(err)
		}
		if len(sections[".text"]) != f.Size {
			t.Fatalf("%s compiles to %d bytes", f.Name, len(sections[".text"]))
		}
		f.Hash = shortHash(sections[".text"])
	}
	build, err := resolveUnitBuild(m, m.Units[0])
	if err != nil {
		t.Fatal(err)
	}
	if build.renames, err = p.unitRenames(resolver, build); err != nil {
		t.Fatal(err)
	}
	sections, err := p.compileUnit(resolver, build)
	if err != nil {
		t.Fatal(err)
	}
	if matched, problems := compareUnit(build, sections); matched != 2 || len(problems) != 0 {
		t.Fatalf("matched %d, problems %v", matched, problems)
	}
	m.Functions[1].Hash = "0000000000000000"
	if matched, problems := compareUnit(build, sections); matched != 1 || len(problems) != 1 || !strings.Contains(problems[0], "main_second differs") {
		t.Fatalf("matched %d, problems %v", matched, problems)
	}
}
