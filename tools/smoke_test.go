package main

import (
	"os"
	"os/exec"
	"path/filepath"
	"testing"
)

func TestSmokeNamesListOwnSymbolsWithCode(t *testing.T) {
	for _, tool := range []string{"mipsel-linux-gnu-as", "mipsel-linux-gnu-ld"} {
		if _, err := exec.LookPath(tool); err != nil {
			t.Skipf("%s is not in PATH", tool)
		}
	}
	dir := t.TempDir()
	source := filepath.Join(dir, "a.s")
	writeTestFile(t, source, []byte(`
	.set noreorder
	.text
	.globl first
	.type first, @function
first:
	jr $ra
	nop
	.size first, 8
	.globl second
	.type second, @function
second:
	addiu $sp, $sp, -8
	jr $ra
	addiu $sp, $sp, 8
	nop
	.size second, 16
	.data
	.globl g_table
g_table:
	.word first
LM1:
	.word 0
`))
	object := filepath.Join(dir, "a.o")
	elfPath := filepath.Join(dir, "a.elf")
	if out, err := exec.Command("mipsel-linux-gnu-as", "-EL", "-march=r3000", "-o", object, source).CombinedOutput(); err != nil {
		t.Fatalf("as: %v\n%s", err, out)
	}
	if out, err := exec.Command("mipsel-linux-gnu-ld", "-EL", "-Ttext=0x801c2500", "--defsym", "elsewhere=0x80010000", "-o", elfPath, object).CombinedOutput(); err != nil {
		t.Fatalf("ld: %v\n%s", err, out)
	}
	names, err := readSmokeNames("effect-e001", elfPath)
	if err != nil {
		t.Fatal(err)
	}
	got := map[string]smokeName{}
	for _, n := range names {
		got[n.name] = n
	}
	if _, ok := got["elsewhere"]; ok {
		t.Fatal("an absolute name another module places is listed")
	}
	if _, ok := got["LM1"]; ok {
		t.Fatal("a local label is listed")
	}
	first, second, table := got["first"], got["second"], got["g_table"]
	if first.address != 0x801c2500 || len(first.code) != 8 || second.address != 0x801c2508 || len(second.code) != 16 {
		t.Fatalf("functions: %+v %+v", first, second)
	}
	if first.code[0] != 0x08 || first.code[3] != 0x03 {
		t.Fatalf("first code % x", first.code)
	}
	if table.module != "effect-e001" || table.code != nil {
		t.Fatalf("data: %+v", table)
	}
}

func TestSmokeShotImageDecodesBothDepths(t *testing.T) {
	// One pixel each: 16-bit pure red then blue, 24-bit green.
	picture, err := smokeShotImage([]byte{0x1f, 0x00, 0x00, 0x7c}, 2, 1, 16)
	if err != nil {
		t.Fatal(err)
	}
	if r, _, b, _ := picture.At(0, 0).RGBA(); r>>8 != 0xf8 || b != 0 {
		t.Fatalf("red pixel %v", picture.At(0, 0))
	}
	if r, _, b, _ := picture.At(1, 0).RGBA(); r != 0 || b>>8 != 0xf8 {
		t.Fatalf("blue pixel %v", picture.At(1, 0))
	}
	picture, err = smokeShotImage([]byte{0, 0xff, 0}, 1, 1, 24)
	if err != nil {
		t.Fatal(err)
	}
	if _, g, _, _ := picture.At(0, 0).RGBA(); g>>8 != 0xff {
		t.Fatalf("green pixel %v", picture.At(0, 0))
	}
	if _, err := smokeShotImage([]byte{0}, 2, 2, 16); err == nil {
		t.Fatal("a short screenshot decoded")
	}
}

func TestSmokeShotsBecomePNGFiles(t *testing.T) {
	dir := t.TempDir()
	raw := filepath.Join(dir, "01-title.2x1x16.raw")
	if err := os.WriteFile(raw, []byte{0x1f, 0, 0, 0x7c}, 0o644); err != nil {
		t.Fatal(err)
	}
	count, err := convertSmokeShots(dir)
	if err != nil || count != 1 {
		t.Fatalf("converted %d: %v", count, err)
	}
	if _, err := os.Stat(filepath.Join(dir, "01-title.png")); err != nil {
		t.Fatal(err)
	}
	if _, err := os.Stat(raw); !os.IsNotExist(err) {
		t.Fatal("the raw screenshot stayed")
	}
}
