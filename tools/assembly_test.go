package main

import (
	"bytes"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

func TestAssemblyFunctionValidation(t *testing.T) {
	row := "  - {addr: 0x80067010, size: 8, name: battle_switch, hash: 0123456789abcdef, asm: \"Test ABI boundary\"}\n"
	source := "/* Switch to the test stack.\n *\n * BATTLE.BIN 0x80067010..0x80067017.\n */\n" +
		"\t.set\tnoreorder\n\t.text\n\t.globl\tbattle_switch\nbattle_switch:\n" +
		"\tlui\t$t0, %hi(g_battle_value)\n\tlui\t$t1, 0x8000 # the sign bit\n\tjr\t$ra\n\tnop\n"
	quirks := "# Quirks\n\n## " + quirksAssemblySection + "\n\nIntro.\n\n- `battle_switch`: test.\n"
	// A region the routine does not cover stays code kept as bytes.
	pending := "\nregions:\n  - {addr: 0x80067020, end: 0x80067030, kind: handwritten, why: \"test\"}\n"
	for _, test := range []struct {
		name, row, path, source, quirks, regions, want string
	}{
		{name: "valid outside main", row: row, source: source, quirks: quirks, regions: pending},
		{name: "blank reason", row: strings.Replace(row, `asm: "Test ABI boundary"`, `asm: " "`, 1), source: source, quirks: quirks, want: "asm needs the reason"},
		{name: "c source", row: strings.Replace(row, `, asm: "Test ABI boundary"`, `, source: src/battle/battle_switch.s`, 1), source: source, quirks: quirks, want: "requires a .c source"},
		{name: "not named after it", row: strings.Replace(row, `asm: "Test`, `source: src/battle/other.s, asm: "Test`, 1), path: "other.s", source: source, quirks: quirks, want: "named after it"},
		{name: "no Port debt entry", row: row, source: source, quirks: "## Other surprises\n\n- `battle_switch`.\n", want: "QUIRKS.md has no"},
		{name: "no opening comment", row: row, source: "\tjr\t$ra\n\tnop\n", quirks: quirks, want: "does not open with a comment"},
		{name: "other range in comment", row: row, source: strings.Replace(source, "0x80067010..", "0x80067014..", 1), quirks: quirks, want: "does not open with a comment"},
		{name: "numeric word", row: row, source: source + "\t.word\t0x80067100\n", quirks: quirks, want: "as a number (0x80067100)"},
		{name: "numeric hi", row: row, source: source + "\tlui\t$t0, %hi(0x80067100)\n", quirks: quirks, want: "as a number (%hi("},
		{name: "numeric upper half", row: row, source: source + "\tlui\t$t0, 0x8006\n", quirks: quirks, want: "as a number (lui"},
		{name: "region left", row: row, source: source, quirks: quirks, regions: "\nregions:\n  - {addr: 0x80067010, end: 0x80067018, kind: handwritten, why: \"test\"}\n", want: "is its source now; remove the region"},
	} {
		t.Run(test.name, func(t *testing.T) {
			p := validTestProject(t)
			path := test.path
			if path == "" {
				path = "battle_switch.s"
			}
			files := map[string]string{
				"src/battle/" + path: test.source,
				"QUIRKS.md":          test.quirks,
				"target/battle.yaml": strings.Replace(testBattleYAML, "\ndata:", test.row+"\ndata:", 1) + test.regions,
			}
			for name, text := range files {
				if err := os.WriteFile(filepath.Join(p.root, filepath.FromSlash(name)), []byte(text), 0o644); err != nil {
					t.Fatal(err)
				}
			}
			config, err := p.loadProjectConfig()
			if err != nil {
				t.Fatal(err)
			}
			if _, err := p.writeProjectConfig(config); err != nil {
				t.Fatal(err)
			}
			if config, err = p.loadProjectConfig(); err != nil {
				t.Fatal(err)
			}
			err = p.validateProjectConfig(config)
			switch {
			case test.want == "" && err != nil:
				t.Fatalf("valid assembly rejected: %v", err)
			case test.want != "" && (err == nil || !strings.Contains(err.Error(), test.want)):
				t.Fatalf("want %q, got %v", test.want, err)
			}
			if test.want == "" {
				battle, _ := config.module("battle")
				if ranges, size := battle.codeKeptAsBytes(); ranges != 1 || size != 16 {
					t.Fatalf("code kept as bytes: %d ranges, %d bytes", ranges, size)
				}
			}
		})
	}
}

func TestSourceInventoryRejectsOrphans(t *testing.T) {
	root := t.TempDir()
	if err := os.MkdirAll(filepath.Join(root, "src"), 0o755); err != nil {
		t.Fatal(err)
	}
	for _, name := range []string{"test.c", "extra.s"} {
		if err := os.WriteFile(filepath.Join(root, "src", name), []byte("nop\n"), 0o600); err != nil {
			t.Fatal(err)
		}
	}
	if err := validateSourceInventory(root, "src", map[string]bool{"src/test.c": true}, "test"); err == nil {
		t.Fatal("orphan assembly accepted")
	}
	if err := validateSourceInventory(root, "src", map[string]bool{"src/test.c": true, "src/extra.s": true}, "test"); err != nil {
		t.Fatal(err)
	}
}

func TestStandaloneAssembly(t *testing.T) {
	if err := (project{}).compilePermuterInput([]string{"fixture.ld", "fixture.s", "out.o", defaultProfile}); err == nil || !strings.Contains(err.Error(), "requires C source") {
		t.Fatalf("assembly permuter callback: %v", err)
	}
	for _, input := range []string{".include \"a.s\"", "label: .incbin \"media.bin\"", ".INCLUDE \"a.s\""} {
		if validateStandaloneAssembly([]byte(input)) == nil {
			t.Fatalf("accepted %s", input)
		}
	}
	if err := validateStandaloneAssembly([]byte("\tjr\t$ra\n\tnop\n")); err != nil {
		t.Fatal(err)
	}
	if _, err := sourceLanguage("object.bin"); err == nil {
		t.Fatal("accepted binary source")
	}
}

// Docker integration fixture: no game bytes or SDK source are needed.
func TestAssemblyBackendAndCacheSeparation(t *testing.T) {
	requirePinnedToolchain(t)
	root := t.TempDir()
	p := project{root: root}
	source, linker := filepath.Join(root, "fixture.s"), filepath.Join(root, "fixture.ld")
	assembly := "\t.text\n\t.set\tnoreorder\n\t.ent\tfixture\nfixture:\n\taddu\t$v0,$a0,$zero\n\tjr\t$ra\n\tnop\n\t.end\tfixture\n"
	if err := os.WriteFile(source, []byte(assembly), 0o600); err != nil {
		t.Fatal(err)
	}
	if err := os.WriteFile(linker, []byte("OUTPUT_ARCH(mips)\nSECTIONS { .text 0x80010000 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) } }\n"), 0o600); err != nil {
		t.Fatal(err)
	}
	profile, _ := compilerProfileNamed(defaultProfile)
	cKey, err := compilationCacheKey(source, linker, profile, "C")
	if err != nil {
		t.Fatal(err)
	}
	aKey, err := compilationCacheKey(source, linker, profile, "asm")
	if err != nil || cKey == aKey {
		t.Fatalf("cache language collision: %v", err)
	}
	t.Setenv("TOOLS_CACHE", "0")
	sections, err := p.compileHistoricalSections(source, linker, filepath.Join(root, "build"), profile)
	if err != nil {
		t.Fatal(err)
	}
	want := []byte{0x21, 0x10, 0x80, 0x00, 0x08, 0x00, 0xe0, 0x03, 0, 0, 0, 0}
	if !bytes.Equal(sections[".text"], want) {
		t.Fatalf("assembly bytes: %x", sections[".text"])
	}
	t.Setenv("TOOLS_CACHE", "1")
	if _, err := p.compileHistoricalSections(source, linker, filepath.Join(root, "cache-fill"), profile); err != nil {
		t.Fatal(err)
	}
	cached, err := p.compileHistoricalSections(source, linker, filepath.Join(root, "cache-hit"), profile)
	if err != nil || !bytes.Equal(cached[".text"], want) {
		t.Fatalf("assembly cache hit: %v", err)
	}
	if _, err := os.Stat(filepath.Join(root, "cache-hit", profile.name+".o")); !os.IsNotExist(err) {
		t.Fatalf("cache hit unexpectedly assembled: %v", err)
	}
	t.Setenv("TOOLS_CACHE", "0")
	changed := append([]byte(nil), want...)
	changed[0] ^= 1
	if _, differs := mismatch(changed, sections[".text"]); !differs {
		t.Fatal("accepted changed expected bytes")
	}
	if _, differs := mismatch(want[:8], sections[".text"]); !differs {
		t.Fatal("accepted wrong size")
	}
	// Allocatable sections must remain visible to the shared production checker.
	if err := os.WriteFile(source, []byte(assembly+"\t.section\t.extra,\"a\"\n\t.word\t7\n"), 0o600); err != nil {
		t.Fatal(err)
	}
	sections, err = p.compileHistoricalSections(source, linker, filepath.Join(root, "extra"), profile)
	if err != nil {
		t.Fatal(err)
	}
	if len(sections[".extra"]) == 0 {
		t.Fatal("extra allocatable section hidden")
	}
}
