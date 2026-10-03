package main

import (
	"io"
	"io/fs"
	"os"
	"path/filepath"
	"reflect"
	"strings"
	"testing"
)

const testMapMainHeader = `#ifndef MAIN_H
#define MAIN_H

/* system */
typedef struct main_record {
    s32 value;
} main_record_t;

typedef enum main_mode {
    MAIN_MODE_IDLE = 0,
    MAIN_MODE_RUN = 1,
} main_mode_e;

typedef char main_record_size_must_be_4[(sizeof(main_record_t) == 4) ? 1 : -1];

extern main_record_t g_main_value;
void main_first(void);

#define MAIN_CALL_SECOND() main_second()

/* sound */
void main_second(void);

#endif
`

const testMapBattleHeader = `/* unit */
extern s32 g_battle_value;
void battle_helper(void);
`

func testMapProject(t *testing.T) project {
	t.Helper()
	p := testProject(t, map[string]string{
		"target/main.yaml":           testMainYAML,
		"target/event.yaml":          testEventYAML,
		"target/battle.yaml":         testBattleYAML,
		"include/fft/main.h":         testMapMainHeader,
		"include/fft/battle.h":       testMapBattleHeader,
		"include/psx/types.h":        "typedef int s32;\n",
		"src/main/main_first.c":      "#include \"fft/main.h\"\n\n/* Runs the second routine through a macro\n * and reads the record. */\nvoid main_first(void) {\n    g_main_value.value = MAIN_MODE_RUN;\n    MAIN_CALL_SECOND();\n}\n",
		"src/main/main_second.c":     "#include \"fft/main.h\"\n\n/* Not a summary: a define follows. */\n#define LOCAL_STEP 1\nvoid main_second(void) { battle_helper(); }\n",
		"src/event/attack_entry.c":   "void attack_entry(void) {\n    void (*handler)(void) = main_second;\n    s32 value = g_battle_value;\n    handler();\n}\n",
		"src/event/card_entry.c":     "/* Port debt (QUIRKS.md): the old card_old_name\n * read a stale register. */\nvoid card_entry(void) {}\n",
		"src/battle/battle_helper.c": "/* main 0x80067000; layout note */\nvoid battle_helper(void) {}\n",
		"QUIRKS.md":                  testMapQuirks,
	})
	p.out = io.Discard
	return p
}

const testMapQuirks = `# Target quirks

Facts about the retail code.

## Retail bugs the source reproduces

- ` + "`main_first`" + `: writes ` + "`g_main_value`" + ` in ` + "`src/main/main_first.c`" + `; the
  ` + "`div`" + ` result and ` + "`main_missing_name`" + ` are named here.
- ` + "`battle_helper`" + ` and its twin ` + "`main_second`" + `.

## Calls that disagree with the callee

- ` + "`card_entry`" + ` passes nothing.
`

func TestCodebaseMapQuirksDebtsAndStale(t *testing.T) {
	p := testMapProject(t)
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	index, err := buildCodebaseIndex(p, config)
	if err != nil {
		t.Fatal(err)
	}
	if len(index.quirks) != 2 || len(index.quirks[0].entries) != 2 || index.quirks[0].intro != "" {
		t.Fatalf("sections %+v", index.quirks)
	}
	first := index.quirks[0].entries[0]
	if first.line != 7 || !strings.Contains(first.text, "\n  `div`") {
		t.Fatalf("first entry at line %d: %q", first.line, first.text)
	}
	if !reflect.DeepEqual(first.missing, []string{"main_missing_name"}) {
		t.Fatalf("missing = %v (instructions such as div are not names)", first.missing)
	}
	for name, want := range map[string]int{"main_first": 1, "main_second": 1, "battle_helper": 1, "card_entry": 1, "attack_entry": 0} {
		if got := len(index.functions[name].quirks); got != want {
			t.Fatalf("%s has %d quirks, want %d", name, got, want)
		}
	}
	if len(index.debts) != 1 || index.debts[0].kind != "port" || index.debts[0].comment.function != "card_entry" {
		t.Fatalf("debts %+v", index.debts)
	}
	var stale []string
	for _, entry := range index.stale {
		stale = append(stale, entry.name)
	}
	if !reflect.DeepEqual(stale, []string{"main_missing_name", "card_old_name"}) {
		t.Fatalf("stale = %v", stale)
	}

	if err := p.mapCommand(nil); err != nil {
		t.Fatal(err)
	}
	pages := readTree(t, filepath.Join(p.root, "build", "map"))
	section := pages["quirks/Retail bugs the source reproduces.md"]
	if !strings.Contains(section, "- [[main_first]]: writes [[g_main_value]] in [[main_first|src/main/main_first.c]];") {
		t.Fatalf("section page:\n%s", section)
	}
	if !strings.Contains(pages["functions/event/card_entry.md"], "- **Port debt**, line 1: Port debt (QUIRKS.md): the old card_old_name read a stale register.") {
		t.Fatalf("card_entry page:\n%s", pages["functions/event/card_entry.md"])
	}
	if !strings.Contains(pages["globals/fft/main/g_main_value.md"], "## Quirks (1)") {
		t.Fatal("global page lacks its quirk")
	}
	backlog := pages["reports/Build 3 backlog.md"]
	for _, want := range []string{"port_debt_comments: 1", "| [[Retail bugs the source reproduces]] | 2 | FIX_BUGS |", "| port | [[card_entry]] | 1 |"} {
		if !strings.Contains(backlog, want) {
			t.Fatalf("backlog lacks %q:\n%s", want, backlog)
		}
	}
	if !strings.Contains(pages["reports/Stale mentions.md"], "| [[card_entry]] | 1 | `card_old_name` |") {
		t.Fatalf("stale page:\n%s", pages["reports/Stale mentions.md"])
	}
}

func TestCodebaseIndexReadsDefinitionsAndReferences(t *testing.T) {
	p := testMapProject(t)
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	index, err := buildCodebaseIndex(p, config)
	if err != nil {
		t.Fatal(err)
	}
	first := index.functions["main_first"]
	if first.summary != "Runs the second routine through a macro and reads the record." {
		t.Fatalf("summary = %q", first.summary)
	}
	if first.line != 5 || first.signature != "void main_first(void)" {
		t.Fatalf("definition at line %d as %q", first.line, first.signature)
	}
	check := func(name string, got, want []string) {
		t.Helper()
		if !reflect.DeepEqual(got, want) {
			t.Fatalf("%s = %v, want %v", name, got, want)
		}
	}
	check("main_first calls", first.calls, []string{"main_second"})
	check("main_first globals", first.globals, []string{"g_main_value"})
	check("main_first types", first.types, []string{"main_mode_e"})

	second := index.functions["main_second"]
	if second.summary != "" {
		t.Fatalf("a comment followed by a define is not a summary: %q", second.summary)
	}
	check("main_second calls", second.calls, []string{"battle_helper"})
	check("main_second called by", second.calledBy, []string{"main_first"})
	check("main_second address used by", second.referencedBy, []string{"attack_entry"})
	check("attack_entry references", index.functions["attack_entry"].references, []string{"main_second"})
	check("attack_entry globals", index.functions["attack_entry"].globals, []string{"g_battle_value"})

	if got := first.prototype; got.header != "include/fft/main.h" || got.label != "system" {
		t.Fatalf("main_first prototype %+v", got)
	}
	if got := second.prototype.label; got != "sound" {
		t.Fatalf("main_second label %q", got)
	}
	if index.types["main_record_t"].kind != "struct" || index.types["main_mode_e"].kind != "enum" {
		t.Fatalf("type kinds %q %q", index.types["main_record_t"].kind, index.types["main_mode_e"].kind)
	}
	if _, ok := index.types["main_record_size_must_be_4"]; ok {
		t.Fatal("size assertion indexed as a type")
	}
	global := index.globals["g_main_value"]
	if len(global.addresses) != 2 || global.addresses[0].module != "battle" || global.addresses[1].module != "main" {
		t.Fatalf("g_main_value addresses %+v", global.addresses)
	}
	if proseSummary(index.functions["battle_helper"].summary) {
		t.Fatal("an address note is not a prose summary")
	}
	if got := mapSubsystem("battle_formula_calculate_hit", "src/battle/battle_formula_calculate_hit.c"); got != "battle_formula" {
		t.Fatalf("subsystem %q", got)
	}
	if got := mapSubsystem("RotTrans", "src/psyq/libgte/RotTrans.c"); got != "psyq_libgte" {
		t.Fatalf("library subsystem %q", got)
	}
}

func readTree(t *testing.T, root string) map[string]string {
	t.Helper()
	files := map[string]string{}
	err := filepath.WalkDir(root, func(path string, entry fs.DirEntry, err error) error {
		if err != nil || entry.IsDir() {
			return err
		}
		data, err := os.ReadFile(path)
		if err != nil {
			return err
		}
		relative, _ := filepath.Rel(root, path)
		files[filepath.ToSlash(relative)] = string(data)
		return nil
	})
	if err != nil {
		t.Fatal(err)
	}
	return files
}

func TestMapCommandWritesDeterministicVault(t *testing.T) {
	p := testMapProject(t)
	if err := p.mapCommand(nil); err != nil {
		t.Fatal(err)
	}
	vault := filepath.Join(p.root, "build", "map")
	first := readTree(t, vault)
	page := first["functions/main/main_first.md"]
	for _, want := range []string{
		"type: \"function\"",
		"has_summary: true",
		"prose_summary: true",
		"header_label: \"system\"",
		"## Calls (1)\n\n[[main_second]]",
		"[[g_main_value]]",
		"[[main_mode_e]]",
		"| [[main (module)\\|main]] | `0x80010000` | 8 |",
	} {
		if !strings.Contains(page, want) {
			t.Fatalf("main_first page lacks %q:\n%s", want, page)
		}
	}
	if !strings.Contains(first["functions/main/main_second.md"], "## Address used by (1)\n\n[[attack_entry]]") {
		t.Fatalf("main_second page:\n%s", first["functions/main/main_second.md"])
	}
	if !strings.Contains(first["functions/event/card_entry.md"], "No C code calls this function") {
		t.Fatal("an uncalled function does not say so")
	}
	for _, path := range []string{"Home.md", "modules/main (module).md", "subsystems/main_first (subsystem).md",
		"headers/main.h.md", "types/fft/main/main_record_t.md", "globals/fft/battle/g_battle_value.md"} {
		if _, ok := first[path]; !ok {
			t.Fatalf("missing %s; have %v", path, sortedKeys(first))
		}
	}

	// Obsidian settings survive a rebuild, and the pages come out identical.
	settings := filepath.Join(vault, ".obsidian", "app.json")
	writeTestFile(t, settings, []byte("{}"))
	if err := p.mapCommand(nil); err != nil {
		t.Fatal(err)
	}
	second := readTree(t, vault)
	if second[".obsidian/app.json"] != "{}" {
		t.Fatal(".obsidian was not kept")
	}
	delete(second, ".obsidian/app.json")
	if !reflect.DeepEqual(first, second) {
		t.Fatal("a second run wrote different pages")
	}
}

func TestMapCommandRefusesOutputOutsideBuild(t *testing.T) {
	p := testMapProject(t)
	for _, out := range []string{"src", "build", "build/..", "/tmp/elsewhere"} {
		if err := p.mapCommand([]string{"--out=" + out}); err == nil {
			t.Fatalf("--out=%s was accepted", out)
		}
	}
	if _, err := os.Stat(filepath.Join(p.root, "src", "main", "main_first.c")); err != nil {
		t.Fatal(err)
	}
}

func TestTableRowEscapesPipes(t *testing.T) {
	if got := tableRow("[[a (module)|a]]", "x | y"); got != "| [[a (module)\\|a]] | x \\| y |\n" {
		t.Fatalf("tableRow = %q", got)
	}
}
