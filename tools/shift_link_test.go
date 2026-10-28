package main

import (
	"encoding/binary"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

func TestLinkScriptOpensTheShiftInsertion(t *testing.T) {
	m := linkTestModule()
	m.ramEnd = 0x80100080
	layout, err := moduleLinkLayout(m)
	if err != nil {
		t.Fatal(err)
	}
	layout.insert = 16
	if data := linkDataAssembly(m, layout, ""); !strings.Contains(data, "\t.section .insert, \"a\", @progbits\n\t.rept 4\n\t.word 0x0000000d\n\t.endr\n") {
		t.Fatalf("filler:\n%s", data)
	}
	script := linkScript(layout, "/w/data.o", nil, "", 0)
	for _, line := range []string{
		"  .insert 0x80100000 : SUBALIGN(4) { *(.insert) }\n",
		"  .bytes_80100000 0x80100010 : SUBALIGN(1) { *(.bytes_80100000) }\n",
		"  .text_80100008 0x80100018 : SUBALIGN(4) { *(.text_80100008) }\n",
		"ASSERT(test_second == 0x80100020, \"test_second must begin at 0x80100020\");\n",
		"g_test_inside = ADDR(.text_80100008) + 0x4;\n",
		// The window stays put while the image grows into its room.
		"<= 0x80100080, \"the image must end by its RAM window's end 0x80100080\");\n",
	} {
		if !strings.Contains(script, line) {
			t.Fatalf("script lacks %q:\n%s", line, script)
		}
	}
	// MAIN keeps its header in place and pads its file to whole sectors.
	main := &moduleSpec{ID: mainModuleID, File: "SCUS_942.21", Load: 0x8000f800, Size: 0x1000, SourceDir: "src/main",
		Functions: []functionSpec{{Addr: 0x80010000, Size: 8, Name: "__SN_ENTRY_POINT"}}}
	if layout, err = moduleLinkLayout(main); err != nil {
		t.Fatal(err)
	}
	if shiftPadding(main, 16) != 2032 || shiftPadding(m, 16) != 0 {
		t.Fatalf("padding %d %d", shiftPadding(main, 16), shiftPadding(m, 16))
	}
	layout.insert, layout.padding = 16, 2032
	if data := linkDataAssembly(main, layout, ""); !strings.Contains(data, "\t.section .padding, \"a\", @progbits\n\t.space 2032\n") {
		t.Fatalf("padding:\n%s", data)
	}
	script = linkScript(layout, "/w/data.o", nil, "", 0)
	for _, line := range []string{
		"  .header_8000f800 0x8000f800 : SUBALIGN(1) { *(.header_8000f800) }\n",
		"  .insert 0x80010000 : SUBALIGN(4) { *(.insert) }\n",
		"  .text_80010000 0x80010010 : SUBALIGN(4) { *(.text_80010000) }\n",
		"  .padding 0x80010810 : SUBALIGN(1) { *(.padding) }\n",
		"__exe_text_start = ADDR(.insert);\n",
		"__exe_text_size = ADDR(.padding) + SIZEOF(.padding) - __exe_text_start;\n",
	} {
		if !strings.Contains(script, line) {
			t.Fatalf("MAIN's script lacks %q:\n%s", line, script)
		}
	}
}

func TestShiftedConfigFitsTheDiscAndRefusesCodeNamedMoves(t *testing.T) {
	p := validTestProject(t)
	load := func() *projectConfig {
		config, err := p.loadProjectConfig()
		if err != nil {
			t.Fatal(err)
		}
		last := config.Disc.Entries[len(config.Disc.Entries)-1]
		end := last.LBA + last.sectors(discModuleFiles(config))
		config.Disc.Entries = append(config.Disc.Entries, discEntry{LBA: end, Dummy: 10, Form: 2, Free: true})
		return config
	}
	config := load()
	before := config.discFiles["BATTLE.BIN"]
	shifted, moved, err := p.shiftedConfig(config, 16)
	if err != nil {
		t.Fatal(err)
	}
	// BATTLE.BIN's 4096 bytes take a third sector and the EVENT directory
	// follows it: it moves to the free run. MAIN pads to three sectors.
	battle, _ := shifted.module("battle")
	if strings.Join(moved, " ") != "BATTLE.BIN" || battle.LBA == before.lba || battle.Size != 4096+16 || shifted.discFiles["SCUS_942.21"].sectors != 3 {
		t.Fatalf("moved %q, battle at %d (was %d), size %d, main %+v", moved, battle.LBA, before.lba, battle.Size, shifted.discFiles["SCUS_942.21"])
	}
	if original, _ := config.module("battle"); original.Size != 4096 {
		t.Fatalf("the link's module grew: %d", original.Size)
	}
	source := "void battle_helper(void) { read(DISC_SECTOR_BATTLE_BIN); }\n"
	if err := os.WriteFile(filepath.Join(p.root, "src", "battle", "battle_helper.c"), []byte(source), 0o644); err != nil {
		t.Fatal(err)
	}
	if _, _, err := p.shiftedConfig(load(), 16); err == nil || !strings.Contains(err.Error(), "BATTLE.BIN would move or grow on the shifted disc, but the code names its sector") {
		t.Fatalf("a code-named move: %v", err)
	}
}

// TestShiftCompareModuleExplainsEachWord shifts a small module by hand: each
// reference moves 16 bytes with what it names, the disc word takes the
// layout's new value, and anything else must stay.
func TestShiftCompareModuleExplainsEachWord(t *testing.T) {
	m := linkTestModule()
	m.Relocations = []relocationSpec{{Addr: 0x80100024, To: "test_first"}}
	m.DiscWords = []discWordSpec{{Addr: 0x80100028, Kind: "sector", File: "SYSTEM.CNF", values: []uint32{30}}}
	layout, err := moduleLinkLayout(m)
	if err != nil {
		t.Fatal(err)
	}
	layout.insert = 16
	put := func(image []byte, offset int, word uint32) { binary.LittleEndian.PutUint32(image[offset:], word) }
	original := make([]byte, m.Size)
	put(original, 0x08, 0x0c040004) // jal test_second
	put(original, 0x10, 0x3c028010) // lui v0, %hi(g_test_a)
	put(original, 0x14, 0x8c420000) // lw v0, %lo(g_test_a)(v0)
	put(original, 0x20, 0x80100008) // .rodata: test_first
	put(original, 0x24, 0x80100008) // relocated: test_first
	put(original, 0x28, 23)         // the disc word
	shifted := func() []byte {
		linked := append(make([]byte, 16), original...)
		for offset := 0; offset < 16; offset += 4 {
			put(linked, offset, shiftFillerWord)
		}
		put(linked, 16+0x08, 0x0c040008)
		put(linked, 16+0x14, 0x8c420010)
		put(linked, 16+0x20, 0x80100018)
		put(linked, 16+0x24, 0x80100018)
		put(linked, 16+0x28, 30)
		return linked
	}
	symbols := map[string]uint32{"test_first": 0x80100018}
	from := func(name string) (uint32, bool) {
		return map[string]uint32{"test_first": 0x80100008}[name], name == "test_first"
	}
	var report shiftReport
	shiftCompareModule(&report, m, layout, original, shifted(), symbols, from)
	if report.code != 2 || report.rodata != 1 || report.data != 1 || report.disc != 1 || len(report.unexplained) != 0 {
		t.Fatalf("report %+v", report)
	}
	for name, edit := range map[string]func([]byte){
		"a data byte":       func(linked []byte) { linked[16+0x30]++ },
		"the filler":        func(linked []byte) { put(linked, 4, 0) },
		"a jump moved more": func(linked []byte) { put(linked, 16+0x08, 0x0c04000c) },
		"the disc word":     func(linked []byte) { put(linked, 16+0x28, 31) },
	} {
		t.Run(name, func(t *testing.T) {
			linked := shifted()
			edit(linked)
			var report shiftReport
			shiftCompareModule(&report, m, layout, original, linked, symbols, from)
			if len(report.unexplained) != 1 {
				t.Fatalf("unexplained %q", report.unexplained)
			}
		})
	}
}
