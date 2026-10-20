package main

import (
	"os"
	"path/filepath"
	"strings"
	"testing"
)

func TestTrialBatchParsesAndValidates(t *testing.T) {
	specs, err := parseTrialBatch([]byte(`trials:
  - {module: battle, entry: battle_helper, source: src/event/attack_entry.c, symbol: attack_entry, subst: {g_main_value: g_battle_value}, includes: [fft/battle.h]}
  - module: main
    entry: main_first
    source: src/main/main_second.c
`))
	if err != nil {
		t.Fatal(err)
	}
	if len(specs) != 2 || specs[0].Subst["g_main_value"] != "g_battle_value" || specs[0].Includes[0] != "fft/battle.h" || specs[1].Symbol != "" {
		t.Fatalf("specs = %+v", specs)
	}
	if specs[1].label() != "main main_first <- src/main/main_second.c" {
		t.Fatalf("label = %q", specs[1].label())
	}
	for name, text := range map[string]string{
		"unknown key":    "trials:\n  - {module: main, entry: main_first, source: a.c, profile: x}\n",
		"missing source": "trials:\n  - {module: main, entry: main_first}\n",
		"empty":          "trials: []\n",
		"defines":        "trials:\n  - {module: main, entry: main_first, source: a.c, defines: {VARIANT_N: 1}}\n",
		"bad subst":      "trials:\n  - {module: main, entry: main_first, source: a.c, subst: {\"a b\": c}}\n",
		"bad include":    "trials:\n  - {module: main, entry: main_first, source: a.c, includes: [\"fft/a.c\"]}\n",
	} {
		if _, err := parseTrialBatch([]byte(text)); err == nil {
			t.Errorf("%s: accepted", name)
		}
	}
}

func TestSubstituteIdentifiersRenamesCodeOnlyInOnePass(t *testing.T) {
	source := []byte("#include \"fft/a.h\"\n/* a stays, b stays */\nint a(void) { return b + ab + \"a b\"[0] + 'a'; }\n")
	got := string(substituteIdentifiers(source, map[string]string{"a": "b", "b": "a"}))
	want := "#include \"fft/a.h\"\n/* a stays, b stays */\nint b(void) { return a + ab + \"a b\"[0] + 'a'; }\n"
	if got != want {
		t.Fatalf("got:\n%s\nwant:\n%s", got, want)
	}
	if string(substituteIdentifiers(source, nil)) != string(source) {
		t.Fatal("empty substitution changed the source")
	}
}

func TestSourceDefinitionSymbolFindsTheSinglePublicFunction(t *testing.T) {
	symbol, err := sourceDefinitionSymbol([]byte("static int helper(void) { return 1; }\nextern int other(void);\nint entry(int n) { return helper() + n; }\n"))
	if err != nil || symbol != "entry" {
		t.Fatalf("symbol = %q, err = %v", symbol, err)
	}
	if _, err := sourceDefinitionSymbol([]byte("int a(void) { return 1; }\nint b(void) { return 2; }\n")); err == nil {
		t.Fatal("two definitions accepted")
	}
}

func TestTrialRunsWithoutTheDiscAndReportsMisses(t *testing.T) {
	p := validTestProject(t)
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	for _, test := range []struct {
		spec trialSpec
		want string
	}{
		{trialSpec{Module: "nowhere", Entry: "x", Source: "src/main/main_first.c"}, "unknown module"},
		{trialSpec{Module: "main", Entry: "missing", Source: "src/main/main_first.c"}, "no function missing"},
		{trialSpec{Module: "main", Entry: "main_first", Source: "../outside.c"}, "inside the project"},
		{trialSpec{Module: "main", Entry: "main_first", Source: "src/main/nowhere.c"}, "no such file"},
	} {
		result := p.runTrial(config, newSymbolResolver(config), map[string][]byte{}, test.spec, 0, 4)
		if result.Status != "error" || !strings.Contains(result.Error, test.want) {
			t.Errorf("%+v: status %s error %q, want %q", test.spec, result.Status, result.Error, test.want)
		}
	}
	if err := p.trialCommand([]string{"--batch=build/missing.yaml"}); err == nil {
		t.Fatal("missing batch accepted")
	}
	if err := p.trialCommand([]string{"--module=main"}); err == nil || !strings.Contains(err.Error(), "usage") {
		t.Fatalf("partial flags: %v", err)
	}
}

func TestTrialDifferencesCountsAlignedRows(t *testing.T) {
	// addiu v0,zero,16 ; jr ra ; nop  versus  addiu v0,zero,17 ; jr ra ; nop
	expected := []byte{0x10, 0x00, 0x02, 0x24, 0x08, 0x00, 0xe0, 0x03, 0, 0, 0, 0}
	compiled := []byte{0x11, 0x00, 0x02, 0x24, 0x08, 0x00, 0xe0, 0x03, 0, 0, 0, 0}
	differing, _, lines := trialDifferences(expected, compiled, 4, nil)
	if differing != 1 || len(lines) != 1 || !strings.Contains(lines[0], "+0x0: li v0,16 | li v0,17") {
		t.Fatalf("differing = %d, lines = %v", differing, lines)
	}
}

// A source matches its own entry's bytes; a candidate that returns another
// constant differs in one instruction, named once the image is present.
func TestTrialMatchesAndNamesTheFirstDifference(t *testing.T) {
	requirePinnedToolchain(t)
	p := validTestProject(t)
	writeTestFile(t, filepath.Join(p.root, "src", "main", "main_first.c"), []byte("int main_first(void) { return 16; }\n"))
	writeTestFile(t, filepath.Join(p.root, "build", "candidate", "first.c"), []byte("int main_first(void) { return 17; }\n"))
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	m, _ := config.module("main")
	t.Setenv("TOOLS_CACHE", "0")
	resolver := newSymbolResolver(config)
	sections, err := p.compileFunction(resolver, m, &m.Functions[0], filepath.Join(p.root, "build", "learn"))
	if err != nil {
		t.Fatal(err)
	}
	text := sections[".text"]
	m.Functions[0].Size, m.Functions[0].Hash = len(text), shortHash(text)
	images := map[string][]byte{}
	same := p.runTrial(config, resolver, images, trialSpec{Module: "main", Entry: "main_first", Source: "src/main/main_first.c"}, 0, 4)
	if same.Status != "match" {
		t.Fatalf("own source: %+v", same)
	}
	other := p.runTrial(config, resolver, images, trialSpec{Module: "main", Entry: "main_first", Source: "build/candidate/first.c"}, 1, 4)
	if other.Status != "differ" || len(other.Lines) != 1 || !strings.Contains(other.Lines[0], "extract the disc") {
		t.Fatalf("candidate without image: %+v", other)
	}
	// With the image present the differing instruction is named.
	image := make([]byte, m.Size)
	copy(image[m.offset(m.Functions[0].Addr):], text)
	images["main"] = image
	other = p.runTrial(config, resolver, images, trialSpec{Module: "main", Entry: "main_first", Source: "build/candidate/first.c"}, 2, 4)
	if other.Status != "differ" || other.Differing != 1 || !strings.Contains(strings.Join(other.Lines, "\n"), "li v0,0x10 | li v0,0x11") {
		t.Fatalf("candidate with image: %+v", other)
	}
	if _, err := os.Stat(filepath.Join(p.root, "build", "trial", "main", "main_first", "t2", "first.c")); err != nil {
		t.Fatalf("trial copy missing: %v", err)
	}
}

// A trial compiles with the entry's profile: the CARD screen's -O0 body
// matches only under -O0.
func TestTrialKeepsTheEntryProfile(t *testing.T) {
	requirePinnedToolchain(t)
	p := validTestProject(t)
	writeTestFile(t, filepath.Join(p.root, "src/event/card_entry.c"), []byte("int card_entry(int value) { return value + 1; }\n"))
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	m, _ := config.module("event-card")
	f := m.function("card_entry")
	t.Setenv("TOOLS_CACHE", "0")
	resolver := newSymbolResolver(config)
	sections, err := p.compileFunction(resolver, m, f, filepath.Join(p.root, "build/learn"))
	if err != nil {
		t.Fatal(err)
	}
	f.Size, f.Hash = len(sections[".text"]), shortHash(sections[".text"])
	result := p.runTrial(config, resolver, map[string][]byte{}, trialSpec{Module: m.ID, Entry: f.Name, Source: m.sourceOf(f)}, 0, 4)
	if result.Status != "match" {
		t.Fatalf("entry profile was lost: %+v", result)
	}
	m.Profile = defaultProfile
	result = p.runTrial(config, resolver, map[string][]byte{}, trialSpec{Module: m.ID, Entry: f.Name, Source: m.sourceOf(f)}, 1, 4)
	if result.Status != "differ" {
		t.Fatalf("an -O2 compile matched the -O0 bytes: %+v", result)
	}
}
