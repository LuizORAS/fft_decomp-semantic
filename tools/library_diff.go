package main

// library-diff measures an unregistered library reconstruction with the same
// compiler/linker pipeline as validate. It writes only ignored scratch output;
// accepting a candidate still requires normal target registration and checks.

import (
	"flag"
	"fmt"
	"os"
	"path/filepath"
	"strings"
)

func libraryCandidate(m *moduleSpec, name, source, profile string, addr uint64, size int) (*functionSpec, error) {
	if !validSymbol(name) || filepath.Base(source) != name+".c" {
		return nil, fmt.Errorf("candidate name must be a C symbol and source must be named %s.c", name)
	}
	if addr > 0xffffffff || addr%4 != 0 || size <= 0 || size%4 != 0 || !m.contains(uint32(addr), size) {
		return nil, fmt.Errorf("candidate range is invalid or outside module %s", m.ID)
	}
	end := addr + uint64(size)
	owned := false
	for _, library := range m.Libraries {
		if addr >= uint64(library.Addr) && end <= uint64(library.End) {
			owned = true
		}
	}
	if !owned {
		return nil, fmt.Errorf("candidate range must be contained in a configured linked library")
	}
	if profile == "" {
		profile = m.Profile
	}
	if _, ok := compilerProfileNamed(profile); !ok {
		return nil, fmt.Errorf("unknown compiler profile %q", profile)
	}
	return &functionSpec{Addr: uint32(addr), Size: size, Name: name, Source: source, Profile: profile}, nil
}

func libraryCandidateRodata(m *moduleSpec, f *functionSpec, addr uint64, size int) error {
	if addr > 0xffffffff || addr%4 != 0 || size <= 0 || size%4 != 0 || !m.contains(uint32(addr), size) {
		return fmt.Errorf("candidate rodata range is invalid or outside module %s", m.ID)
	}
	if addr < uint64(f.Addr)+uint64(f.Size) && uint64(f.Addr) < addr+uint64(size) {
		return fmt.Errorf("candidate rodata overlaps its text")
	}
	f.Rodata = []rodataSpec{{Addr: uint32(addr), Size: size}}
	return nil
}

func (p project) libraryDiffCommand(args []string) error {
	flags := flag.NewFlagSet("library-diff", flag.ContinueOnError)
	module := flags.String("module", "main", "original disc module")
	name := flags.String("name", "", "original function symbol")
	source := flags.String("source", "", "candidate C source inside the project")
	address := flags.String("addr", "", "original function address")
	size := flags.Int("size", 0, "original function byte count")
	profile := flags.String("profile", "", "historical compiler profile (default module profile)")
	rodataAddress := flags.String("rodata-addr", "", "original address of source-generated .rodata")
	rodataSize := flags.Int("rodata-size", 0, "original .rodata byte count (requires --rodata-addr)")
	quiet := flags.Bool("quiet", false, "omit assembly listing on a mismatch")
	if err := flags.Parse(args); err != nil {
		return err
	}
	if flags.NArg() != 0 {
		return fmt.Errorf("library-diff accepts named flags only")
	}
	addr, err := parseHex(*address, "candidate address")
	if err != nil {
		return err
	}
	config, err := p.loadProjectConfig()
	if err != nil {
		return err
	}
	m, err := resolveModuleName(config, *module)
	if err != nil {
		return err
	}
	f, err := libraryCandidate(m, *name, *source, *profile, addr, *size)
	if err != nil {
		return err
	}
	if (*rodataAddress == "") != (*rodataSize == 0) {
		return fmt.Errorf("--rodata-addr and --rodata-size must be supplied together")
	}
	if *rodataAddress != "" {
		rodataAddr, err := parseHex(*rodataAddress, "candidate rodata address")
		if err != nil {
			return err
		}
		if err := libraryCandidateRodata(m, f, rodataAddr, *rodataSize); err != nil {
			return err
		}
	}
	if _, err := projectPath(p.root, f.Source, "candidate source"); err != nil {
		return err
	}
	if !p.validExtraction() {
		if err := p.extractDisc(); err != nil {
			return err
		}
	}
	image, err := p.readModuleImage(m)
	if err != nil {
		return err
	}
	expected := image[m.offset(f.Addr) : m.offset(f.Addr)+f.Size]
	workDir := filepath.Join(p.root, "build", "library-diff", m.ID)
	unlock, err := fileLock(filepath.Join(workDir, f.Name+".lock"))
	if err != nil {
		return err
	}
	defer unlock()
	sections, err := p.compileFunction(newSymbolResolver(config), m, f, workDir)
	if err != nil {
		return fmt.Errorf("compile %s: %w", f.Name, err)
	}
	compiled := sections[".text"]
	var problems []string
	if offset, differs := mismatch(expected, compiled); differs {
		listing, err := asmDiffer(filepath.Join(workDir, f.Name), expected, compiled)
		if err != nil {
			return err
		}
		if !*quiet {
			os.Stdout.Write(listing)
		}
		fmt.Fprint(p.stdout(), formatDiffSummary(expected, compiled))
		fmt.Fprint(p.stdout(), mismatchDiagnostics(expected, compiled, offset, uint64(f.Addr)))
		problems = append(problems, fmt.Sprintf(".text differs at +0x%x (%d original bytes, %d compiled)", offset, len(expected), len(compiled)))
	}
	for _, rodata := range f.Rodata {
		start := m.offset(rodata.Addr)
		want := image[start : start+rodata.Size]
		got := sections[".rodata"]
		if offset, differs := mismatch(want, got); differs {
			problems = append(problems, fmt.Sprintf(".rodata differs at +0x%x (%d original bytes, %d compiled)", offset, len(want), len(got)))
		}
	}
	if section := undeclaredSection(f, sections); section != "" {
		problems = append(problems, "emitted undeclared allocatable section "+section)
	}
	if len(problems) != 0 {
		return fmt.Errorf("%s %s does not match: %s", m.ID, f.Name, strings.Join(problems, "; "))
	}
	fmt.Fprintf(p.stdout(), "matches original: %s %s (0x%08x, %d bytes, profile %s)\n", m.ID, f.Name, f.Addr, f.Size, m.profileOf(f))
	for _, rodata := range f.Rodata {
		fmt.Fprintf(p.stdout(), "matches original data: .rodata (0x%08x, %d bytes)\n", rodata.Addr, rodata.Size)
	}
	return nil
}
