package main

import (
	"bytes"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

func TestHeaderMacroObjectBindings(t *testing.T) {
	requirePinnedToolchain(t)
	p := validTestProject(t)
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	m, _ := config.module("main")
	f := &m.Functions[0]
	source := filepath.Join(p.root, filepath.FromSlash(m.sourceOf(f)))
	header := filepath.Join(filepath.Dir(source), "hidden.h")
	if err := os.WriteFile(header, []byte("extern int g_main_value;\n#define hidden_value() g_main_value\n"), 0o600); err != nil {
		t.Fatal(err)
	}
	if err := os.WriteFile(source, []byte("#include \"hidden.h\"\nint main_first(void) { return hidden_value(); }\n"), 0o600); err != nil {
		t.Fatal(err)
	}
	resolver := newSymbolResolver(config)
	work := filepath.Join(p.root, "build", "macro")
	macroSections, err := p.compileFunction(resolver, m, f, work)
	if err != nil {
		t.Fatal(err)
	}
	script, err := os.ReadFile(filepath.Join(work, f.Name, "generated.ld"))
	if err != nil || !strings.Contains(string(script), "PROVIDE(g_main_value = 0x80010100)") {
		t.Fatalf("missing object binding: %s (%v)", script, err)
	}
	if err := os.WriteFile(source, []byte("extern int g_main_value;\nint main_first(void) { return g_main_value; }\n"), 0o600); err != nil {
		t.Fatal(err)
	}
	directSections, err := p.compileFunction(resolver, m, f, filepath.Join(p.root, "build", "direct"))
	if err != nil {
		t.Fatal(err)
	}
	if !bytes.Equal(macroSections[".text"], directSections[".text"]) {
		t.Fatalf("macro bytes %x differ from direct bytes %x", macroSections[".text"], directSections[".text"])
	}
}

func TestHeaderMacroUnknownSymbolStillFails(t *testing.T) {
	requirePinnedToolchain(t)
	p := validTestProject(t)
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	m, _ := config.module("main")
	f := &m.Functions[0]
	source := filepath.Join(p.root, filepath.FromSlash(m.sourceOf(f)))
	if err := os.WriteFile(filepath.Join(filepath.Dir(source), "hidden.h"), []byte("extern int missing_binding;\n#define hidden_value() missing_binding\n"), 0o600); err != nil {
		t.Fatal(err)
	}
	if err := os.WriteFile(source, []byte("#include \"hidden.h\"\nint main_first(void) { return hidden_value(); }\n"), 0o600); err != nil {
		t.Fatal(err)
	}
	if _, err := p.compileFunction(newSymbolResolver(config), m, f, filepath.Join(p.root, "build")); err == nil {
		t.Fatal("unresolved header symbol was accepted")
	}
}
