package main

import (
	"debug/elf"
	"encoding/binary"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

// The second original entry keeps its own name/profile while compiling the
// first entry's definition. Only fixture files are removed or rewritten.
func sharedDefinitionProject(t *testing.T, body string) project {
	t.Helper()
	p := validTestProject(t)
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	m, _ := config.module("main")
	source := m.sourceOf(&m.Functions[0])
	other := m.sourceOf(&m.Functions[1])
	m.Functions[1].Source = source
	m.Functions[1].SourceSymbol = m.Functions[0].Name
	if err := os.Remove(filepath.Join(p.root, other)); err != nil {
		t.Fatal(err)
	}
	writeSymbolRenameFixture(t, p.root, source, body)
	if _, err := p.writeProjectConfig(config); err != nil {
		t.Fatal(err)
	}
	return p
}

func checkedFixtureConfig(t *testing.T, p project) *projectConfig {
	t.Helper()
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	if err := p.validateProjectConfig(config); err != nil {
		t.Fatal(err)
	}
	return config
}

func TestSourceSymbolDefaultsAndCanonicalRoundTrip(t *testing.T) {
	f := &functionSpec{Name: "entry"}
	if f.sourceSymbol() != "entry" {
		t.Fatal("default source symbol changed")
	}
	f.SourceSymbol = "body"
	if f.sourceSymbol() != "body" {
		t.Fatal("explicit source symbol ignored")
	}
	p := sharedDefinitionProject(t, "void main_first(void) {}\n")
	config := checkedFixtureConfig(t, p)
	m, _ := config.module("main")
	m.Functions[0].SourceSymbol = m.Functions[0].Name
	rendered := string(renderModuleConfig(m))
	if strings.Count(rendered, "source_symbol:") != 1 {
		t.Fatalf("redundant symbol not omitted:\n%s", rendered)
	}
	reloaded, err := parseModuleConfig([]byte(rendered))
	if err != nil {
		t.Fatal(err)
	}
	if reloaded.Functions[0].SourceSymbol != "" || reloaded.Functions[1].SourceSymbol != "main_first" {
		t.Fatalf("source symbols did not round trip: %#v", reloaded.Functions)
	}
	for index, original := range m.Functions {
		got := reloaded.Functions[index]
		if got.Addr != original.Addr || got.Size != original.Size || got.Hash != original.Hash || m.profileOf(&got) != m.profileOf(&original) || got.Name != original.Name {
			t.Fatalf("original entry contract changed: %#v -> %#v", original, got)
		}
	}
}

func TestSharedSourceDefinitionValidation(t *testing.T) {
	for name, tc := range map[string]struct{ body, symbol, want string }{
		"public body with private helper": {"static int helper(void) { return 1; }\nvoid main_first(void) { helper(); }\n", "main_first", ""},
		"old style body":                  {"main_first(value) int value; { return value; }\n", "main_first", ""},
		"prototype only":                  {"void main_first(void);\n", "main_first", "exactly one public function"},
		"static body":                     {"static void main_first(void) {}\n", "main_first", "exactly one public function"},
		"wrong body":                      {"void other(void) {}\n", "main_first", "exactly one public function"},
		"two public bodies":               {"void main_first(void) {}\nvoid other(void) {}\n", "main_first", "exactly one public function"},
		"invalid symbol":                  {"void main_first(void) {}\n", "not-a-symbol", "invalid source_symbol"},
		"conflicting symbol":              {"void main_first(void) {}\n", "other", "conflicting definition symbols"},
		"implicit different definition":   {"void main_first(void) {}\n", "", "conflicting definition symbols"},
	} {
		t.Run(name, func(t *testing.T) {
			p := sharedDefinitionProject(t, tc.body)
			config, err := p.loadProjectConfig()
			if err != nil {
				t.Fatal(err)
			}
			m, _ := config.module("main")
			m.Functions[1].SourceSymbol = tc.symbol
			if _, err := p.writeProjectConfig(config); err != nil {
				t.Fatal(err)
			}
			config, err = p.loadProjectConfig()
			if err != nil {
				t.Fatal(err)
			}
			err = p.validateProjectConfig(config)
			if tc.want == "" {
				if err != nil {
					t.Fatal(err)
				}
			} else if err == nil || !strings.Contains(err.Error(), tc.want) {
				t.Fatalf("validation = %v, want %q", err, tc.want)
			}
		})
	}
}

func TestSharedSourceRetainsEntryValidation(t *testing.T) {
	for name, tc := range map[string]struct {
		edit func(*functionSpec)
		want string
	}{
		"hash":     {func(f *functionSpec) { f.Hash = "invalid" }, "no valid hash"},
		"range":    {func(f *functionSpec) { f.Addr = 0x80010000 }, "overlaps"},
		"size":     {func(f *functionSpec) { f.Size = 2 }, "invalid or out-of-image range"},
		"profile":  {func(f *functionSpec) { f.Profile = "unknown" }, "unknown profile"},
		"name":     {func(f *functionSpec) { f.Name = "main_first" }, "duplicates"},
		"assembly": {func(f *functionSpec) { f.Asm = "architectural entry" }, "source_symbol requires a C source"},
	} {
		t.Run(name, func(t *testing.T) {
			p := sharedDefinitionProject(t, "void main_first(void) {}\n")
			config, err := p.loadProjectConfig()
			if err != nil {
				t.Fatal(err)
			}
			m, _ := config.module("main")
			tc.edit(&m.Functions[1])
			if _, err := p.writeProjectConfig(config); err != nil {
				t.Fatal(err)
			}
			config, err = p.loadProjectConfig()
			if err != nil {
				t.Fatal(err)
			}
			if err := p.validateProjectConfig(config); err == nil || !strings.Contains(err.Error(), tc.want) {
				t.Fatalf("validation = %v, want %q", err, tc.want)
			}
		})
	}
}

func TestSharedSourceSelfAndExternalBindings(t *testing.T) {
	p := sharedDefinitionProject(t, "extern int g_main_value;\nint main_first(int n) { return n ? main_first(n - 1) : g_main_value; }\n")
	config := checkedFixtureConfig(t, p)
	m, _ := config.module("main")
	m.Overrides = []overrideSpec{{Function: "main_second", Name: "g_main_value", Addr: 0x80010300}}
	resolver := newSymbolResolver(config)
	used := map[string]struct{}{"main_first": {}, "main_second": {}, "g_main_value": {}}
	first := resolver.linkerBindings(m, &m.Functions[0], used)
	second := resolver.linkerBindings(m, &m.Functions[1], used)
	if first["main_first"] != m.Functions[0].Addr || second["main_first"] != m.Functions[1].Addr || second["g_main_value"] != 0x80010300 || first["g_main_value"] != 0x80010100 {
		t.Fatalf("per-entry self/external bindings: %v / %v", first, second)
	}
	// A separate caller retains both original public entry addresses.
	caller := &functionSpec{Name: "caller", Addr: 0x80010400}
	bindings := resolver.linkerBindings(m, caller, used)
	if bindings["main_first"] != m.Functions[0].Addr || bindings["main_second"] != m.Functions[1].Addr {
		t.Fatalf("public entries collapsed in caller: %v", bindings)
	}
	m.Functions[0].SourceSymbol = "shared_body"
	m.Functions[1].SourceSymbol = "shared_body"
	resolver = newSymbolResolver(config)
	if _, exists := resolver.resolve(m, "shared_body"); exists {
		t.Fatal("definition was invented as a public entry")
	}
	if bindings := resolver.linkerBindings(m, &m.Functions[1], map[string]struct{}{"shared_body": {}}); bindings["shared_body"] != m.Functions[1].Addr {
		t.Fatalf("unexported definition has no per-entry self binding: %v", bindings)
	}
}

func TestSharedSourceRejectsConflictingSelfOverride(t *testing.T) {
	p := sharedDefinitionProject(t, "void main_first(void) {}\n")
	config := checkedFixtureConfig(t, p)
	m, _ := config.module("main")
	m.Overrides = []overrideSpec{{Function: "main_second", Name: "main_first", Addr: m.Functions[0].Addr}}
	if _, err := p.writeProjectConfig(config); err != nil {
		t.Fatal(err)
	}
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	if err := p.validateProjectConfig(config); err == nil || !strings.Contains(err.Error(), "conflicts with its source definition") {
		t.Fatalf("self override was accepted: %v", err)
	}
}

func TestSourceSymbolRenameEntryKeepsSharedDefinition(t *testing.T) {
	p := sharedDefinitionProject(t, "void main_first(void) {}\n")
	writeSymbolRenameFixture(t, p.root, "include/fft/memory.h", "void main_first(void);\nvoid main_second(void);\n")
	writeSymbolRenameFixture(t, p.root, "src/battle/battle_helper.c", "void battle_helper(void) { main_first(); main_second(); }\n")
	before := readTestFile(t, p, "src/main/main_first.c")
	if err := p.symbolsCommand([]string{"rename-function", "--old", "main_second", "--new", "main_again", "--dry-run"}); err != nil {
		t.Fatal(err)
	}
	if readTestFile(t, p, "src/main/main_first.c") != before {
		t.Fatal("dry run changed shared body")
	}
	if err := p.symbolsCommand([]string{"rename-function", "--old", "main_second", "--new", "main_again"}); err != nil {
		t.Fatal(err)
	}
	config := checkedFixtureConfig(t, p)
	m, _ := config.module("main")
	f := m.function("main_again")
	if f == nil || f.sourceSymbol() != "main_first" || m.sourceOf(f) != "src/main/main_first.c" {
		t.Fatalf("entry rename changed source ownership: %#v", f)
	}
	if readTestFile(t, p, "src/main/main_first.c") != before {
		t.Fatal("entry rename changed shared body")
	}
	if _, err := os.Stat(filepath.Join(p.root, "src/main/main_again.c")); !os.IsNotExist(err) {
		t.Fatalf("entry rename moved shared body: %v", err)
	}
	if got := readTestFile(t, p, "src/battle/battle_helper.c"); !strings.Contains(got, "main_first(); main_again();") {
		t.Fatalf("caller not renamed: %s", got)
	}
}

func TestSourceSymbolRenameDefinitionUpdatesAllOwners(t *testing.T) {
	for _, implementationOnly := range []bool{false, true} {
		t.Run(map[bool]string{false: "public definition", true: "implementation only"}[implementationOnly], func(t *testing.T) {
			p := sharedDefinitionProject(t, "void main_first(void) {}\n")
			config := checkedFixtureConfig(t, p)
			m, _ := config.module("main")
			battle, _ := config.module("battle")
			old := "main_first"
			if implementationOnly {
				old = "shared_body"
				m.Functions[0].SourceSymbol = old
				m.Functions[1].SourceSymbol = old
				writeSymbolRenameFixture(t, p.root, "src/main/main_first.c", "void shared_body(void) {}\n")
			}
			battle.Functions[0].Source = "src/main/main_first.c"
			battle.Functions[0].SourceSymbol = old
			if err := os.Remove(filepath.Join(p.root, "src/battle/battle_helper.c")); err != nil {
				t.Fatal(err)
			}
			if _, err := p.writeProjectConfig(config); err != nil {
				t.Fatal(err)
			}
			if err := p.symbolsCommand([]string{"rename-function", "--old", old, "--new", "mem_copy"}); err != nil {
				t.Fatal(err)
			}
			config = checkedFixtureConfig(t, p)
			for _, id := range []string{"main", "battle"} {
				module, _ := config.module(id)
				for index := range module.Functions {
					f := &module.Functions[index]
					if f.sourceSymbol() != "mem_copy" || module.sourceOf(f) != "src/main/mem_copy.c" {
						t.Fatalf("owner not updated: %s %#v", id, f)
					}
				}
			}
			m, _ = config.module("main")
			if m.function("main_second") == nil || m.Functions[1].Addr != 0x80010008 || m.Functions[1].Profile != "gcc-2.6.3_O1_aspsx-2.34" {
				t.Fatal("original alias contract changed")
			}
			if implementationOnly && m.function("main_first") == nil {
				t.Fatal("implementation rename changed public entry")
			}
			if got := readTestFile(t, p, "src/main/mem_copy.c"); got != "void mem_copy(void) {}\n" {
				t.Fatalf("definition body not renamed: %s", got)
			}
			if _, err := os.Stat(filepath.Join(p.root, "src/main/main_first.c")); !os.IsNotExist(err) {
				t.Fatal("old shared source remains")
			}
		})
	}
}

func TestSharedDefinitionCompilationRecursionAndCallback(t *testing.T) {
	requirePinnedToolchain(t)
	p := sharedDefinitionProject(t, `extern int g_main_value;
extern int consume(int (*callback)(int), int value);
int main_first(int n) {
    if (n <= 0) return g_main_value;
    return consume(main_first, main_first(n - 1));
}
`)
	config := checkedFixtureConfig(t, p)
	m, _ := config.module("main")
	m.Functions[1].Addr = 0x80010080
	m.Imports = append(m.Imports, symbolSpec{Name: "consume", Addr: 0x80011000})
	m.Overrides = []overrideSpec{{Function: "main_second", Name: "consume", Addr: 0x80012000}, {Function: "main_second", Name: "g_main_value", Addr: 0x80010300}}
	resolver := newSymbolResolver(config)
	t.Setenv("TOOLS_CACHE", "0")
	work := filepath.Join(p.root, "build", "shared-body")
	for index := range m.Functions {
		f := &m.Functions[index]
		sections, err := p.compileFunction(resolver, m, f, work)
		if err != nil {
			t.Fatal(err)
		}
		linked, err := elf.Open(filepath.Join(work, f.Name, m.profileOf(f)+".elf"))
		if err != nil {
			t.Fatal(err)
		}
		symbols, err := linked.Symbols()
		linked.Close()
		if err != nil {
			t.Fatal(err)
		}
		values := map[string]uint64{}
		for _, symbol := range symbols {
			values[symbol.Name] = symbol.Value
		}
		wantConsume, wantGlobal := uint64(0x80011000), uint64(0x80010100)
		if index == 1 {
			wantConsume, wantGlobal = 0x80012000, 0x80010300
		}
		if values["main_first"] != uint64(f.Addr) || values["consume"] != wantConsume || values["g_main_value"] != wantGlobal {
			t.Fatalf("%s linked definition/bindings: %v", f.Name, values)
		}
		selfCall, externalCall := false, false
		for offset := 0; offset+4 <= len(sections[".text"]); offset += 4 {
			word := binary.LittleEndian.Uint32(sections[".text"][offset:])
			if word>>26 == 3 {
				destination := uint64((f.Addr+uint32(offset)+4)&0xf0000000) | uint64(word&0x03ffffff)<<2
				selfCall = selfCall || destination == uint64(f.Addr)
				externalCall = externalCall || destination == wantConsume
			}
		}
		if !selfCall || !externalCall {
			t.Fatalf("%s lost recursive/external calls: %x", f.Name, sections[".text"])
		}
		object, err := elf.Open(filepath.Join(work, f.Name, m.profileOf(f)+".o"))
		if err != nil {
			t.Fatal(err)
		}
		objectSymbols, err := object.Symbols()
		if err != nil {
			object.Close()
			t.Fatal(err)
		}
		relocations, err := object.Section(".rel.text").Data()
		object.Close()
		if err != nil {
			t.Fatal(err)
		}
		hi, lo := false, false
		for offset := 0; offset+8 <= len(relocations); offset += 8 {
			info := binary.LittleEndian.Uint32(relocations[offset+4:])
			symbolIndex := int(info>>8) - 1
			if symbolIndex >= 0 && symbolIndex < len(objectSymbols) && objectSymbols[symbolIndex].Name == "main_first" {
				hi = hi || elf.R_MIPS(info&0xff) == elf.R_MIPS_HI16
				lo = lo || elf.R_MIPS(info&0xff) == elf.R_MIPS_LO16
			}
		}
		if !hi || !lo {
			t.Fatalf("%s callback lacks definition-address relocations", f.Name)
		}
	}
	// Callers still bind each public name to its original entry, rather than
	// routing both names through the canonical definition's first instance.
	caller := &functionSpec{Name: "caller", Addr: 0x80010400}
	writeSymbolRenameFixture(t, p.root, m.sourceOf(caller), `extern int main_first(int n);
extern int main_second(int n);
int caller(int n) { return main_first(n) + main_second(n); }
`)
	sections, err := p.compileFunction(resolver, m, caller, work)
	if err != nil {
		t.Fatal(err)
	}
	calls := map[uint32]bool{}
	for offset := 0; offset+4 <= len(sections[".text"]); offset += 4 {
		word := binary.LittleEndian.Uint32(sections[".text"][offset:])
		if word>>26 == 3 {
			calls[((caller.Addr+uint32(offset)+4)&0xf0000000)|(word&0x03ffffff)<<2] = true
		}
	}
	if !calls[m.Functions[0].Addr] || !calls[m.Functions[1].Addr] {
		t.Fatalf("caller entries collapsed: %v", calls)
	}
}

func TestPermuterSharedDefinitionUsesConsistentScoringSymbol(t *testing.T) {
	f := &functionSpec{Name: "original_entry", SourceSymbol: "shared_body"}
	target := permuterTargetAssembly(f)
	settings := permuterSettings(f.sourceSymbol())
	if !strings.Contains(target, "\nshared_body:\n") || !strings.Contains(settings, `func_name = "shared_body"`) || strings.Contains(target, "original_entry") || strings.Contains(settings, "original_entry") {
		t.Fatalf("target/candidate scoring identities disagree: %s / %s", target, settings)
	}
	f.SourceSymbol = ""
	if !strings.Contains(permuterTargetAssembly(f), "\noriginal_entry:\n") {
		t.Fatal("default scoring symbol changed")
	}
}

func TestSharedDefinitionHeaderBindingRetryKeepsEntryOverride(t *testing.T) {
	requirePinnedToolchain(t)
	p := sharedDefinitionProject(t, "#include \"hidden.h\"\nint main_first(void) { return hidden_value(); }\n")
	writeSymbolRenameFixture(t, p.root, "src/main/hidden.h", "extern int g_main_value;\n#define hidden_value() g_main_value\n")
	config := checkedFixtureConfig(t, p)
	m, _ := config.module("main")
	m.Overrides = []overrideSpec{{Function: "main_second", Name: "g_main_value", Addr: 0x80010300}}
	t.Setenv("TOOLS_CACHE", "0")
	work := filepath.Join(p.root, "build", "shared-macro")
	resolver := newSymbolResolver(config)
	for index := range m.Functions {
		f := &m.Functions[index]
		if _, err := p.compileFunction(resolver, m, f, work); err != nil {
			t.Fatal(err)
		}
		script, err := os.ReadFile(filepath.Join(work, f.Name, "generated.ld"))
		if err != nil {
			t.Fatal(err)
		}
		want := "PROVIDE(g_main_value = 0x80010100)"
		if index == 1 {
			want = "PROVIDE(g_main_value = 0x80010300)"
		}
		if !strings.Contains(string(script), want) {
			t.Fatalf("%s object fallback lost entry override: %s", f.Name, script)
		}
	}
}

func TestSourceSymbolBatchRenamesKeepSourceOwners(t *testing.T) {
	for _, order := range []string{"main_first\tmem_copy\nmain_second\tmem_copy_alias\n", "main_second\tmem_copy_alias\nmain_first\tmem_copy\n"} {
		t.Run(strings.Split(order, "\t")[0], func(t *testing.T) {
			p := sharedDefinitionProject(t, "void main_first(void) {}\n")
			writeSymbolRenameFixture(t, p.root, "renames.tsv", order)
			if err := p.symbolsCommand([]string{"rename-function", "--pairs", "renames.tsv"}); err != nil {
				t.Fatal(err)
			}
			config := checkedFixtureConfig(t, p)
			m, _ := config.module("main")
			for _, name := range []string{"mem_copy", "mem_copy_alias"} {
				f := m.function(name)
				if f == nil || f.sourceSymbol() != "mem_copy" || m.sourceOf(f) != "src/main/mem_copy.c" {
					t.Fatalf("batch lost %s ownership: %#v", name, f)
				}
			}
			if got := readTestFile(t, p, "src/main/mem_copy.c"); got != "void mem_copy(void) {}\n" {
				t.Fatalf("definition = %s", got)
			}
		})
	}
}

func TestSourceSymbolEntryRenamePreservesImplicitPath(t *testing.T) {
	p := sharedDefinitionProject(t, "void main_first(void) {}\n")
	config := checkedFixtureConfig(t, p)
	m, _ := config.module("main")
	oldPath := m.sourceOf(&m.Functions[0])
	newPath := "src/main/main_second.c"
	writeSymbolRenameFixture(t, p.root, newPath, readTestFile(t, p, oldPath))
	if err := os.Remove(filepath.Join(p.root, oldPath)); err != nil {
		t.Fatal(err)
	}
	m.Functions[0].Source = newPath
	m.Functions[1].Source = ""
	if _, err := p.writeProjectConfig(config); err != nil {
		t.Fatal(err)
	}
	if err := p.symbolsCommand([]string{"rename-function", "--old", "main_second", "--new", "main_again"}); err != nil {
		t.Fatal(err)
	}
	config = checkedFixtureConfig(t, p)
	m, _ = config.module("main")
	f := m.function("main_again")
	if f == nil || f.Source != newPath || m.sourceOf(f) != newPath || f.sourceSymbol() != "main_first" {
		t.Fatalf("implicit source path moved with alias: %#v", f)
	}
}

func TestSharedDefinitionRejectsLeadingPrivateCode(t *testing.T) {
	requirePinnedToolchain(t)
	p := sharedDefinitionProject(t, `static int helper(int n) { return n + 1; }
int main_first(int n) { return helper(n); }
`)
	config := checkedFixtureConfig(t, p)
	m, _ := config.module("main")
	f := &m.Functions[1]
	f.Profile = "gcc-2.6.3_O0_aspsx-2.34"
	t.Setenv("TOOLS_CACHE", "0")
	if _, err := p.compileFunction(newSymbolResolver(config), m, f, filepath.Join(p.root, "build", "leading-private-code")); err == nil || !strings.Contains(err.Error(), "source definition must begin at original entry") {
		t.Fatalf("definition entry shifted behind private helper: %v", err)
	}
}

func TestSharedDefinitionCannotUseProvidedAddressForMissingBody(t *testing.T) {
	requirePinnedToolchain(t)
	p := sharedDefinitionProject(t, "void other(void) {}\n")
	// Exercise the compiled-object guard independently of source validation.
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	m, _ := config.module("main")
	f := &m.Functions[1]
	source, script, err := p.functionLinker(newSymbolResolver(config), m, f)
	if err != nil {
		t.Fatal(err)
	}
	if !strings.HasSuffix(source, "main_first.c") || strings.Contains(script, "PROVIDE(main_first =") || !strings.Contains(script, "ASSERT(main_first == ") {
		t.Fatalf("mapped definition was claimed by linker script: %s", script)
	}
	t.Setenv("TOOLS_CACHE", "0")
	if _, err := p.compileFunction(newSymbolResolver(config), m, f, filepath.Join(p.root, "build", "missing-mapped-body")); err == nil || !strings.Contains(err.Error(), "undefined symbol `main_first'") {
		t.Fatalf("missing object definition passed mapped entry guard: %v", err)
	}
}
