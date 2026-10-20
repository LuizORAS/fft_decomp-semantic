// unit_check.go rebuilds original translation units from their members'
// per-function sources and compiles each as one object (`make unit-check`).
// The units are the `units:` rows of target/; the merged sources and their
// builds go under build/units/, and nothing else changes. A unit that matches
// shows that its members' C still builds as the one file it was: in this
// order, under one profile, with one address per name.
package main

import (
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"
	"sync"
)

// unitBuild is one unit resolved for a compile.
type unitBuild struct {
	module  *moduleSpec
	unit    unitSpec
	members []*functionSpec
	profile compilerProfile
	rodata  []rodataSpec // the members' literal ranges, in address order
	tables  []unitTable  // read-only data the original file defined among them
	notes   []string     // members compiled alone under another profile
	problem string       // why the unit cannot compile from its members
	// renames maps a member to the identifiers its body takes in the unit:
	// its definition's entry name, and for each override the name of the
	// symbol at the override's address.
	renames map[string]map[string]string
}

// unitTable is read-only data the unit's original file defined between its
// members' literals. The data stays in the disc image, so the unit check
// copies it from there into the merged source under build/, before the
// member whose literals follow it.
type unitTable struct {
	addr   uint32
	size   int
	before string
}

// unitTableSource defines one table from the module image, as words when it
// is word-aligned (GCC aligns an array to a word either way).
func unitTableSource(m *moduleSpec, table unitTable, image []byte) string {
	data := image[m.offset(table.addr) : m.offset(table.addr)+table.size]
	var values []string
	kind := "unsigned int"
	if table.addr%4 == 0 && table.size%4 == 0 {
		for offset := 0; offset < len(data); offset += 4 {
			values = append(values, fmt.Sprintf("0x%08x", binary.LittleEndian.Uint32(data[offset:])))
		}
	} else {
		kind = "unsigned char"
		for _, value := range data {
			values = append(values, fmt.Sprintf("0x%02x", value))
		}
	}
	return fmt.Sprintf("/* Read-only data the original file defined at 0x%08x, from the disc image. */\nconst %s unit_check_data_%08x[%d] = { %s };",
		table.addr, kind, table.addr, len(values), strings.Join(values, ", "))
}

// nameAt names the symbol at addr the way resolution searches: the module's
// own names, then each linked module's, then main's (the first name in
// order when several share the address).
func (resolver symbolResolver) nameAt(m *moduleSpec, addr uint32) string {
	first := func(names map[string]uint32) string {
		found := ""
		for name, at := range names {
			if at == addr && (found == "" || name < found) {
				found = name
			}
		}
		return found
	}
	if name := first(resolver.own[m.ID]); name != "" {
		return name
	}
	for _, link := range m.Links {
		if name := first(resolver.exported[link]); name != "" {
			return name
		}
	}
	return first(resolver.exported[mainModuleID])
}

// unitRenames gives each member the names its body takes in the unit: its
// definition's entry name, and the names one object cannot share. A source
// compiled at several entries binds some names per entry through overrides;
// when another member uses such a name at another address (or is that
// name), the override's body names the symbol at its address instead, as
// the original file must have. An override with no symbol at its address
// stays a binding.
func (p project) unitRenames(resolver symbolResolver, build *unitBuild) (map[string]map[string]string, error) {
	m := build.module
	used := map[string]map[string]struct{}{}
	for _, f := range build.members {
		names, err := sourceIdentifiers(filepath.Join(p.root, filepath.FromSlash(m.sourceOf(f))))
		if err != nil {
			return nil, err
		}
		used[f.Name] = names
	}
	overrides := map[string]map[string]uint32{}
	for _, override := range m.Overrides {
		if overrides[override.Function] == nil {
			overrides[override.Function] = map[string]uint32{}
		}
		overrides[override.Function][override.Name] = override.Addr
	}
	bound := func(f *functionSpec, name string) (uint32, bool) {
		if addr, ok := overrides[f.Name][name]; ok {
			return addr, true
		}
		return resolver.resolve(m, name)
	}
	renames := map[string]map[string]string{}
	for _, f := range build.members {
		names := map[string]string{}
		if symbol := f.sourceSymbol(); symbol != f.Name {
			names[symbol] = f.Name
		}
		for name, addr := range overrides[f.Name] {
			shared := true
			for _, other := range build.members {
				if other == f {
					continue
				}
				if other.Name == name && other.Addr != addr {
					shared = false
				}
				if _, uses := used[other.Name][name]; uses {
					if at, ok := bound(other, name); !ok || at != addr {
						shared = false
					}
				}
			}
			if !shared {
				if symbol := resolver.nameAt(m, addr); symbol != "" && symbol != name {
					names[name] = symbol
				}
			}
		}
		renames[f.Name] = names
	}
	return renames, nil
}

// unitCheckResult is the outcome of one unit.
type unitCheckResult struct {
	build    *unitBuild
	matched  int
	problems []string
}

// sameProfileClass reports whether two profiles differ at most in the
// division checks, a flag of the whole build that leaves a function without
// a division unchanged.
func sameProfileClass(a, b compilerProfile) bool {
	return a.compilerPath == b.compilerPath && a.optimization == b.optimization &&
		a.aspsxVersion == b.aspsxVersion && a.smallDataLimit == b.smallDataLimit
}

// resolveUnitBuild picks the unit's profile (its row's, or the module
// default, in the checked-division variant when a member uses that) and
// collects its literal ranges and the tables between them.
func resolveUnitBuild(m *moduleSpec, unit unitSpec) (*unitBuild, error) {
	build := &unitBuild{module: m, unit: unit, members: m.unitMembers(unit)}
	owner := map[uint32]string{}
	if len(build.members) == 0 {
		return nil, fmt.Errorf("%s: unit %s has no functions", m.ID, unit.Name)
	}
	profile, ok := compilerProfileNamed(m.unitProfileName(unit))
	if !ok {
		return nil, fmt.Errorf("%s: unit %s uses unknown profile %q", m.ID, unit.Name, m.unitProfileName(unit))
	}
	for _, f := range build.members {
		own, ok := compilerProfileNamed(m.profileOf(f))
		if !ok {
			return nil, fmt.Errorf("%s: %s uses unknown profile %q", m.ID, f.Name, m.profileOf(f))
		}
		switch {
		case !sameProfileClass(own, profile):
			build.notes = append(build.notes, fmt.Sprintf("%s compiles alone under %s", f.Name, own.name))
		case own.expandDiv && !profile.expandDiv:
			profile = own
		}
		for _, span := range f.Rodata {
			// Members that use one literal each declare it, compiled alone; the
			// unit's object holds it once, so a range inside a declared one
			// is that literal again.
			shared := false
			for _, existing := range build.rodata {
				if span.Addr >= existing.Addr && span.Addr+uint32(span.Size) <= existing.Addr+uint32(existing.Size) {
					shared = true
					break
				}
			}
			if !shared {
				build.rodata = append(build.rodata, span)
				owner[span.Addr] = f.Name
			}
		}
	}
	build.profile = profile
	sort.Slice(build.rodata, func(i, j int) bool { return build.rodata[i].Addr < build.rodata[j].Addr })
	for i := 1; i < len(build.rodata); i++ {
		end := build.rodata[i-1].Addr + uint32(build.rodata[i-1].Size)
		next := build.rodata[i].Addr
		// Short alignment gaps between members' literals are allowed; the
		// section length and each range's hash check their placement. A
		// longer one held a table of the original file, word-aligned.
		switch {
		case next < end:
			build.problem = fmt.Sprintf("the members' .rodata ranges overlap at 0x%08x", next)
		case next-end >= 8:
			start := (end + 3) &^ 3
			build.tables = append(build.tables, unitTable{addr: start, size: int(next - start), before: owner[next]})
		}
	}
	return build, nil
}

// mergeUnitSources concatenates the members' sources in address order: the
// include lines first, once each in first-seen order, then the bodies, each
// with its unit renames (a shared source's neutral definition takes the
// entry's name, as the original object named it), and the unit's tables
// from image where the original file defined them.
func mergeUnitSources(root string, build *unitBuild, image []byte) ([]byte, error) {
	var includes, bodies []string
	seen := map[string]bool{}
	for _, f := range build.members {
		for _, table := range build.tables {
			if table.before == f.Name {
				bodies = append(bodies, unitTableSource(build.module, table, image))
			}
		}
		data, err := os.ReadFile(filepath.Join(root, filepath.FromSlash(build.module.sourceOf(f))))
		if err != nil {
			return nil, err
		}
		data = substituteIdentifiers(data, build.renames[f.Name])
		var body []string
		for _, line := range strings.Split(string(data), "\n") {
			if includeDirectiveLine.MatchString(line) {
				if trimmed := strings.TrimSpace(line); !seen[trimmed] {
					seen[trimmed] = true
					includes = append(includes, trimmed)
				}
				continue
			}
			body = append(body, line)
		}
		bodies = append(bodies, strings.TrimSpace(strings.Join(body, "\n")))
	}
	return []byte(strings.Join(includes, "\n") + "\n\n" + strings.Join(bodies, "\n\n") + "\n"), nil
}

// unitBindings binds the names the merged source uses the way the linker
// does for the module, then the members' overrides: one object has one
// address per name, so members whose overrides disagree could not have been
// one object. The members' own names are left to the object.
func unitBindings(resolver symbolResolver, build *unitBuild, used map[string]struct{}) (map[string]uint32, error) {
	m := build.module
	bindings := map[string]uint32{}
	for name, addr := range resolver.own[m.ID] {
		if compilerEmitted(name) {
			bindings[name] = addr
		}
	}
	for name := range used {
		if addr, ok := resolver.resolve(m, name); ok {
			bindings[name] = addr
		}
	}
	owner := map[string]string{}
	for _, f := range build.members {
		for _, override := range m.Overrides {
			if override.Function != f.Name {
				continue
			}
			if _, renamed := build.renames[f.Name][override.Name]; renamed {
				continue
			}
			if previous, ok := owner[override.Name]; ok && bindings[override.Name] != override.Addr {
				return nil, fmt.Errorf("%s and %s bind %s to 0x%08x and 0x%08x", previous, f.Name, override.Name, bindings[override.Name], override.Addr)
			}
			bindings[override.Name], owner[override.Name] = override.Addr, f.Name
		}
	}
	for _, f := range build.members {
		if addr, ok := bindings[f.Name]; ok && owner[f.Name] != "" && addr != f.Addr {
			return nil, fmt.Errorf("%s binds member %s to 0x%08x, but it is at 0x%08x", owner[f.Name], f.Name, addr, f.Addr)
		}
		delete(bindings, f.Name)
	}
	if build.profile.smallDataLimit != 0 {
		bindings["_gp"] = build.profile.globalPointer
	}
	return bindings, nil
}

// unitLinkerScript places the merged .text (and .rodata) at the unit's
// addresses and requires every member to begin at its original entry.
func unitLinkerScript(build *unitBuild, bindings map[string]uint32) string {
	names := make([]string, 0, len(bindings))
	for name := range bindings {
		names = append(names, name)
	}
	sort.Strings(names)
	var script strings.Builder
	script.WriteString("OUTPUT_ARCH(mips)\n")
	for _, name := range names {
		if name == "_gp" {
			fmt.Fprintf(&script, "_gp = 0x%08x;\n", bindings[name])
			continue
		}
		fmt.Fprintf(&script, "PROVIDE(%s = 0x%08x);\n", name, bindings[name])
	}
	script.WriteString("SECTIONS {\n")
	if len(build.rodata) > 0 {
		fmt.Fprintf(&script, "  .rodata 0x%x : SUBALIGN(4) { *(.rodata) *(.rdata) }\n", build.rodata[0].Addr)
	}
	fmt.Fprintf(&script, "  .text 0x%x : SUBALIGN(4) { *(.text) }\n", build.unit.Addr)
	script.WriteString("  /DISCARD/ : { *(.reginfo) *(.pdr) *(.comment) *(.gnu.attributes) *(.MIPS.abiflags) }\n}\n")
	for _, f := range build.members {
		fmt.Fprintf(&script, "ASSERT(%s == 0x%08x, \"%s must begin at 0x%08x\");\n", f.Name, f.Addr, f.Name, f.Addr)
	}
	return script.String()
}

// compileUnit writes the merged source under build/units/<module>/ and runs
// the historical pipeline on it, binding the object's actual undefined
// symbols once more when a header macro hid a name, as compileFunction does.
func (p project) compileUnit(resolver symbolResolver, build *unitBuild) (map[string][]byte, error) {
	base := filepath.Join(p.root, "build", "units", build.module.ID)
	source := filepath.Join(base, build.unit.Name+".c")
	var image []byte
	if len(build.tables) > 0 {
		var err error
		if image, err = p.readModuleImage(build.module); err != nil {
			return nil, fmt.Errorf("its file defined %d tables among the literals, read from the extracted disc: %w", len(build.tables), err)
		}
	}
	merged, err := mergeUnitSources(p.root, build, image)
	if err != nil {
		return nil, err
	}
	if err := os.MkdirAll(base, 0o755); err != nil {
		return nil, err
	}
	if err := atomicWrite(source, merged); err != nil {
		return nil, err
	}
	used, err := sourceIdentifiers(source)
	if err != nil {
		return nil, err
	}
	bindings, err := unitBindings(resolver, build, used)
	if err != nil {
		return nil, err
	}
	script := unitLinkerScript(build, bindings)
	linker, err := writeLinkerScript(base, build.unit.Name, script)
	if err != nil {
		return nil, err
	}
	buildDir := filepath.Join(base, build.unit.Name)
	sections, err := p.compileHistoricalSections(source, linker, buildDir, build.profile)
	if err == nil {
		return sections, nil
	}
	object := filepath.Join(buildDir, build.profile.name+".o")
	if undefined, objectErr := undefinedObjectIdentifiers(object); objectErr == nil {
		for name := range used {
			undefined[name] = struct{}{}
		}
		if expanded, bindErr := unitBindings(resolver, build, undefined); bindErr == nil {
			if rescript := unitLinkerScript(build, expanded); rescript != script {
				if _, writeErr := writeLinkerScript(base, build.unit.Name, rescript); writeErr == nil {
					return p.compileHistoricalSections(source, linker, buildDir, build.profile)
				}
			}
		}
	}
	return nil, err
}

// compareUnit slices the merged .text by the members' addresses and sizes
// and applies the hash criterion of `make check`; the .rodata ranges, the
// section sizes and any other section are unit-level problems.
func compareUnit(build *unitBuild, sections map[string][]byte) (int, []string) {
	text := sections[".text"]
	matched := 0
	var problems []string
	for _, f := range build.members {
		offset := int(f.Addr - build.unit.Addr)
		switch {
		case offset+f.Size > len(text):
			problems = append(problems, fmt.Sprintf("%s: the compiled .text ends after %d bytes, before it", f.Name, len(text)))
		case shortHash(text[offset:offset+f.Size]) != f.Hash:
			problems = append(problems, fmt.Sprintf("%s differs (hash %s, want %s)", f.Name, shortHash(text[offset:offset+f.Size]), f.Hash))
		default:
			matched++
		}
	}
	if size := int(build.unit.End - build.unit.Addr); len(text) != size {
		problems = append(problems, fmt.Sprintf("compiled .text is %d bytes, the unit covers %d", len(text), size))
	}
	if len(build.rodata) > 0 {
		compiled := sections[".rodata"]
		last := build.rodata[len(build.rodata)-1]
		if total := int(last.Addr + uint32(last.Size) - build.rodata[0].Addr); len(compiled) != total {
			problems = append(problems, fmt.Sprintf("compiled .rodata is %d bytes, the members' ranges cover %d", len(compiled), total))
		} else {
			for _, span := range build.rodata {
				start := int(span.Addr - build.rodata[0].Addr)
				if shortHash(compiled[start:start+span.Size]) != span.Hash {
					problems = append(problems, fmt.Sprintf(".rodata at 0x%08x differs", span.Addr))
				}
			}
		}
	}
	for _, name := range sortedKeys(sections) {
		if name != ".text" && (name != ".rodata" || len(build.rodata) == 0) {
			problems = append(problems, "emitted undeclared allocatable section "+name)
		}
	}
	return matched, problems
}

func (p project) unitCheckCommand(args []string) error {
	flags := flag.NewFlagSet("unit-check", flag.ContinueOnError)
	selection := flags.String("module", "", "modules to check (default: all)")
	only := flags.String("unit", "", "check only the units with this name")
	if err := flags.Parse(args); err != nil {
		return err
	}
	if flags.NArg() != 0 {
		return errors.New("usage: tools unit-check [--module=M] [--unit=NAME]")
	}
	config, err := p.loadProjectConfig()
	if err != nil {
		return err
	}
	modules := config.Modules
	if *selection != "" {
		if modules, err = resolveModuleSelection(config, *selection); err != nil {
			return err
		}
	}
	resolver := newSymbolResolver(config)
	var builds []*unitBuild
	for _, m := range modules {
		for _, unit := range m.Units {
			if *only != "" && unit.Name != *only {
				continue
			}
			build, err := resolveUnitBuild(m, unit)
			if err != nil {
				return err
			}
			if build.renames, err = p.unitRenames(resolver, build); err != nil {
				return err
			}
			builds = append(builds, build)
		}
	}
	if len(builds) == 0 {
		return errors.New("unit-check: no unit selected")
	}
	jobs, err := toolJobs()
	if err != nil {
		return err
	}
	results := make([]unitCheckResult, len(builds))
	next := make(chan int)
	var wait sync.WaitGroup
	for range min(jobs, len(builds)) {
		wait.Add(1)
		go func() {
			defer wait.Done()
			for index := range next {
				result := unitCheckResult{build: builds[index]}
				if builds[index].problem != "" {
					result.problems = []string{builds[index].problem}
				} else if sections, err := p.compileUnit(resolver, builds[index]); err != nil {
					result.problems = []string{"compile: " + firstLines(err.Error(), 6)}
				} else {
					result.matched, result.problems = compareUnit(builds[index], sections)
				}
				results[index] = result
			}
		}()
	}
	for index := range builds {
		next <- index
	}
	close(next)
	wait.Wait()

	units, functions, matchedUnits, matchedFunctions := len(results), 0, 0, 0
	for _, result := range results {
		build := result.build
		functions += len(build.members)
		matchedFunctions += result.matched
		label := fmt.Sprintf("%s:%s (%d functions, %d bytes, %s)", build.module.ID, build.unit.Name, len(build.members), build.unit.End-build.unit.Addr, build.profile.name)
		if len(result.problems) == 0 {
			matchedUnits++
			fmt.Fprintf(p.stdout(), "ok   %s\n", label)
		} else {
			fmt.Fprintf(p.stdout(), "FAIL %s\n", label)
			for _, problem := range result.problems {
				fmt.Fprintf(p.stdout(), "     %s\n", strings.ReplaceAll(problem, "\n", "\n     "))
			}
		}
		for _, note := range build.notes {
			fmt.Fprintf(p.stdout(), "     note: %s\n", note)
		}
	}
	fmt.Fprintf(p.stdout(), "unit-check: %d of %d units match (%d of %d functions)\n", matchedUnits, units, matchedFunctions, functions)
	if matchedUnits != units {
		return fmt.Errorf("unit-check: %d units differ", units-matchedUnits)
	}
	return nil
}
