package main

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

func TestValidateAddressLiteralsFindsRAMAddressesInCode(t *testing.T) {
	root := t.TempDir()
	write := func(name, text string) {
		path := filepath.Join(root, filepath.FromSlash(name))
		if err := os.MkdirAll(filepath.Dir(path), 0o755); err != nil {
			t.Fatal(err)
		}
		if err := os.WriteFile(path, []byte(text), 0o644); err != nil {
			t.Fatal(err)
		}
	}
	// A header names a fixed area with a #define; a mask, a comment and a
	// string are no address.
	write("include/fft/test.h", "/* buffer at 0x801d0000 */\n#define TEST_BUFFER ((u8*)0x801d0000)\n#define TEST_FLAG 0x80000000U\nextern int g_test;\n")
	write("src/test/clean.c", "#include \"fft/test.h\"\n/* 0x80123456 in a comment */\nint f(void) { const char* s = \"0x80123456\"; return g_test & 0x80000000; }\n")
	if err := validateAddressLiterals(root); err != nil {
		t.Fatalf("clean tree: %v", err)
	}
	// Code, a #define in a .c and a header directive other than #define all count.
	write("src/test/literal.c", "#include \"fft/test.h\"\nvoid g(void) {\n    *(int*)0x8019aa00 = 1;\n}\n")
	write("src/test/define.c", "#define LOCAL ((u8*)0x801EE000u) /* staging */\nvoid h(void) {}\n")
	write("include/fft/if.h", "#if TEST_ADDRESS == 0x80067000\n#endif\n")
	found, err := findAddressLiterals(root, declarationScanDirs)
	if err != nil {
		t.Fatal(err)
	}
	var got []string
	for _, literal := range found {
		got = append(got, fmt.Sprintf("%s:%d 0x%08x", literal.path, literal.line, literal.value))
	}
	want := []string{"include/fft/if.h:1 0x80067000", "src/test/define.c:1 0x801ee000", "src/test/literal.c:3 0x8019aa00"}
	if strings.Join(got, "|") != strings.Join(want, "|") {
		t.Fatalf("found %q, want %q", got, want)
	}
	if err := validateAddressLiterals(root); err == nil || !strings.Contains(err.Error(), "3 RAM addresses written as numbers") {
		t.Fatalf("validate: %v", err)
	}
}
