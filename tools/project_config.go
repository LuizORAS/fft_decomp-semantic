// project_config.go loads, validates and canonically writes target/*.yaml, one
// YAML document per disc module, plus the disc layout (disc_layout.go); tools
// read and write only through it, so hand edits and tool output share one
// spelling (`make config-fmt`).
package main

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strconv"
	"strings"

	"gopkg.in/yaml.v3"
)

const (
	modulesConfigDir = "target"
	hashDigits       = 16
	mainModuleID     = "main"
)

type projectConfig struct {
	Modules []*moduleSpec
	Disc    *discLayout // target/disc.yaml; nil when the checkout has none
	byID    map[string]*moduleSpec
	files   map[string][]byte // module files as loaded, for the canonical-form check
}

// moduleSpec is one disc file. Runtime address addr maps to file offset
// addr - Load; for the PS-X EXE, Load is the address of its 0x800-byte header.
type moduleSpec struct {
	Comment   string // leading comment block, kept verbatim
	ID        string
	File      string // path on the disc
	LBA       int    // the file's first sector, from target/disc.yaml
	Size      int
	SHA256    string
	Load      uint32
	SourceDir string
	Profile   string   // default compiler profile of the module's functions
	Links     []string // modules resident alongside this one, searched after it
	Libraries []librarySpec
	Units     []unitSpec
	Functions []functionSpec
	Data      []symbolSpec
	Imports   []symbolSpec
	Overrides []overrideSpec
	Regions   []regionSpec
	// Relocations and Values classify the data words that look like RAM
	// addresses: a relocation is written from its symbol at link time, a
	// value keeps its original bytes.
	Relocations []relocationSpec
	Values      []valueSpec

	path string // file path relative to the project root
}

type functionSpec struct {
	Addr         uint32
	Size         int
	Name         string
	Hash         string
	Profile      string // empty: the module profile
	Source       string // empty: <source_dir>/<name>.c, or .s when Asm is set
	SourceSymbol string // empty: Name; C definition compiled separately for this entry
	Asm          string // why this function is standalone assembly
	Rodata       []rodataSpec
}

type rodataSpec struct {
	Addr uint32
	Size int
	Hash string
}

type symbolSpec struct {
	Addr uint32
	Name string
}

// overrideSpec binds one name to an address for one function only, ahead of
// the module-wide resolution.
type overrideSpec struct {
	Function string
	Name     string
	Addr     uint32
}

type librarySpec struct {
	ID      string
	Addr    uint32
	End     uint32
	Kind    string
	Library string
}

type regionSpec struct {
	Addr uint32
	End  uint32
	Kind string
	Why  string
}

// relocationSpec says the data word at Addr holds the address of To plus
// Offset; To is any name the module resolves (its own, its links', main's).
type relocationSpec struct {
	Addr   uint32
	To     string
	Offset uint32
}

// valueSpec says the words at [Addr, End) are values, not addresses, for the
// reason Why. End is 0 for the one word at Addr.
type valueSpec struct {
	Addr uint32
	End  uint32
	Why  string
}

func (v valueSpec) end() uint32 {
	if v.End == 0 {
		return v.Addr + 4
	}
	return v.End
}

// unitSpec is one original translation unit: the functions in [Addr, End),
// compiled together from one source file under Profile (empty: the module
// default). Each function keeps its own source; `make unit-check` rebuilds
// the unit from them.
type unitSpec struct {
	Addr    uint32
	End     uint32
	Name    string
	Profile string
}

var libraryKinds = map[string]bool{"psyq": true, "c-runtime": true, "middleware": true}

// regionKinds are the reasons a code range is not reconstructed as C.
var regionKinds = map[string]bool{
	"blocked":     true, // ordinary C the pinned toolchain cannot reproduce
	"data":        true,
	"handwritten": true, // original assembly
	"interior":    true, // a label inside another routine, not an entry
	"library":     true,
	"padding":     true,
}

func (m *moduleSpec) end() uint64 { return uint64(m.Load) + uint64(m.Size) }

// offset converts a runtime address to a file offset in the module image.
func (m *moduleSpec) offset(addr uint32) int { return int(addr - m.Load) }

func (m *moduleSpec) contains(addr uint32, size int) bool {
	return addr >= m.Load && uint64(addr)+uint64(size) <= m.end()
}

// isLibraryFunction reports whether f lies in one of the module's linked
// library ranges.
func (m *moduleSpec) isLibraryFunction(f *functionSpec) bool {
	for _, library := range m.Libraries {
		if f.Addr < library.End && uint64(f.Addr)+uint64(f.Size) > uint64(library.Addr) {
			return true
		}
	}
	return false
}

func (m *moduleSpec) profileOf(f *functionSpec) string {
	if f.Profile != "" {
		return f.Profile
	}
	return m.Profile
}

func (m *moduleSpec) defaultSource(f *functionSpec) string {
	extension := ".c"
	if f.Asm != "" {
		extension = ".s"
	}
	return m.SourceDir + "/" + f.Name + extension
}

// sourceOf returns the function's source path relative to the project root.
func (m *moduleSpec) sourceOf(f *functionSpec) string {
	if f.Source != "" {
		return f.Source
	}
	return m.defaultSource(f)
}

// codeAt names what code [start, end) overlaps: a function's .text or
// .rodata, or MAIN's PS-X EXE header; "" for data.
func (m *moduleSpec) codeAt(start, end uint32) string {
	if m.ID == mainModuleID && start < m.Load+psxEXEHeaderSize {
		return "the PS-X EXE header"
	}
	for _, f := range m.Functions {
		if start < f.Addr+uint32(f.Size) && f.Addr < end {
			return f.Name
		}
		for _, rodata := range f.Rodata {
			if start < rodata.Addr+uint32(rodata.Size) && rodata.Addr < end {
				return f.Name + "'s .rodata"
			}
		}
	}
	return ""
}

// quirksAssemblySection is the QUIRKS.md section that lists each hand-written
// routine's Port debt, a bullet opening with its name in backticks.
const quirksAssemblySection = "Hand-written assembly"

func quirksAssemblyEntries(root string) (map[string]bool, error) {
	data, err := os.ReadFile(filepath.Join(root, mapQuirksFile))
	if err != nil && !os.IsNotExist(err) {
		return nil, err
	}
	names := map[string]bool{}
	for _, section := range parseQuirks(string(data)) {
		if section.title != quirksAssemblySection {
			continue
		}
		for _, entry := range section.entries {
			if match := mapBacktickPattern.FindStringSubmatch(entry.text); match != nil {
				names[match[1]] = true
			}
		}
	}
	return names, nil
}

var (
	// assemblyCommentPattern matches the comments of a .s source: C block
	// comments and the assembler's own `#` line comments.
	assemblyCommentPattern = regexp.MustCompile(`(?s)/\*.*?\*/|#[^\n]*`)
	// assemblyAddressPattern finds a RAM address written as a number: a word,
	// a %hi/%lo of a number, or a lui of an address's upper half. 0x80000000
	// and a lui of 0x8000 are the sign bit, not an address.
	assemblyAddressPattern = regexp.MustCompile(`(?i)\b0x80[01][0-9a-f]{5}\b|%(?:hi|lo)\(\s*0x|\blui\s+\$\w+\s*,\s*0x80(?:0[1-9a-f]|1[0-9a-f])\b`)
)

// validateAssemblySource checks a hand-written routine's source: it opens with
// a comment that names its original start address, and every address in its
// code is a symbol.
func validateAssemblySource(root, source string, f *functionSpec) error {
	data, err := os.ReadFile(filepath.Join(root, filepath.FromSlash(source)))
	if err != nil {
		return err
	}
	text := strings.TrimSpace(string(data))
	end := strings.Index(text, "*/")
	if !strings.HasPrefix(text, "/*") || end < 0 || !strings.Contains(strings.ToLower(text[:end]), fmt.Sprintf("0x%08x", f.Addr)) {
		return fmt.Errorf("%s does not open with a comment naming its original range from 0x%08x", source, f.Addr)
	}
	for _, found := range assemblyAddressPattern.FindAllString(assemblyCommentPattern.ReplaceAllString(text, " "), -1) {
		if !strings.EqualFold(found, "0x80000000") {
			return fmt.Errorf("%s writes an address as a number (%s); name its symbol", source, found)
		}
	}
	return nil
}

// unitMembers lists the functions inside the unit's range, in address order.
func (m *moduleSpec) unitMembers(unit unitSpec) []*functionSpec {
	var members []*functionSpec
	for index := range m.Functions {
		if f := &m.Functions[index]; f.Addr >= unit.Addr && f.Addr < unit.End {
			members = append(members, f)
		}
	}
	sort.SliceStable(members, func(i, j int) bool { return members[i].Addr < members[j].Addr })
	return members
}

func (m *moduleSpec) unitProfileName(unit unitSpec) string {
	if unit.Profile != "" {
		return unit.Profile
	}
	return m.Profile
}

// regionOverlapping returns the first region overlapping [start, end).
func (m *moduleSpec) regionOverlapping(start, end uint32) (regionSpec, bool) {
	for _, region := range m.Regions {
		if start < region.End && region.Addr < end {
			return region, true
		}
	}
	return regionSpec{}, false
}

func (m *moduleSpec) function(name string) *functionSpec {
	for index := range m.Functions {
		if m.Functions[index].Name == name {
			return &m.Functions[index]
		}
	}
	return nil
}

func (c *projectConfig) module(id string) (*moduleSpec, bool) {
	m, ok := c.byID[id]
	return m, ok
}

// moduleConfigPath is the file that holds a module id. Event overlays share
// target/event.yaml and effect overlays target/effect.yaml, one YAML document
// per module.
func moduleConfigPath(id string) string {
	switch {
	case strings.HasPrefix(id, "event-"):
		return modulesConfigDir + "/event.yaml"
	case strings.HasPrefix(id, "effect-"):
		return modulesConfigDir + "/effect.yaml"
	}
	return modulesConfigDir + "/" + id + ".yaml"
}

// loadProjectConfig reads every module file and the disc layout, which gives
// each module's file its sector. It checks syntax and the facts every
// consumer relies on; validateProjectConfig checks the rest.
func (p project) loadProjectConfig() (*projectConfig, error) {
	config := &projectConfig{byID: make(map[string]*moduleSpec), files: make(map[string][]byte)}
	paths, err := filepath.Glob(filepath.Join(p.root, filepath.FromSlash(modulesConfigDir), "*.yaml"))
	if err != nil {
		return nil, err
	}
	sort.Strings(paths)
	for _, path := range paths {
		data, err := os.ReadFile(path)
		if err != nil {
			return nil, err
		}
		relative, err := filepath.Rel(p.root, path)
		if err != nil {
			return nil, err
		}
		relative = filepath.ToSlash(relative)
		config.files[relative] = data
		if relative == discLayoutPath {
			if config.Disc, err = parseDiscLayout(data); err != nil {
				return nil, fmt.Errorf("%s: %w", relative, err)
			}
			continue
		}
		for index, document := range splitYAMLDocuments(data) {
			module, err := parseModuleConfig(document)
			if err != nil {
				return nil, fmt.Errorf("%s document %d: %w", relative, index+1, err)
			}
			module.path = relative
			if module.ID == "" || !validModuleID(module.ID) || moduleConfigPath(module.ID) != relative {
				return nil, fmt.Errorf("%s: module id %q belongs in %s", relative, module.ID, moduleConfigPath(module.ID))
			}
			if _, exists := config.byID[module.ID]; exists {
				return nil, fmt.Errorf("duplicate module %s", module.ID)
			}
			config.byID[module.ID] = module
			config.Modules = append(config.Modules, module)
		}
	}
	if _, ok := config.byID[mainModuleID]; !ok {
		return nil, fmt.Errorf("%s has no main module", modulesConfigDir)
	}
	if config.Disc != nil {
		modules := discModuleFiles(config)
		for _, entry := range config.Disc.Entries {
			if m := modules[entry.File]; m != nil && entry.File != "" {
				m.LBA = entry.LBA
			}
		}
	}
	return config, nil
}

// splitYAMLDocuments splits a stream at `---` lines, keeping each document's
// leading comment with it.
func splitYAMLDocuments(data []byte) [][]byte {
	var documents [][]byte
	var current []string
	for _, line := range strings.SplitAfter(string(data), "\n") {
		if strings.TrimRight(line, "\r\n") == "---" {
			documents = append(documents, []byte(strings.Join(current, "")))
			current = nil
			continue
		}
		current = append(current, line)
	}
	if len(current) > 0 || len(documents) == 0 {
		documents = append(documents, []byte(strings.Join(current, "")))
	}
	return documents
}

type moduleFileYAML struct {
	Module    string         `yaml:"module"`
	File      string         `yaml:"file"`
	Size      int            `yaml:"size"`
	SHA256    string         `yaml:"sha256"`
	Load      uint32         `yaml:"load"`
	SourceDir string         `yaml:"source_dir"`
	Profile   string         `yaml:"profile"`
	Links     []string       `yaml:"links"`
	Libraries []libraryYAML  `yaml:"libraries"`
	Units     []unitYAML     `yaml:"units"`
	Functions []functionYAML `yaml:"functions"`
	Data      []symbolYAML   `yaml:"data"`
	Imports   []symbolYAML   `yaml:"imports"`
	Overrides []overrideYAML `yaml:"overrides"`
	Regions   []regionYAML   `yaml:"regions"`

	Relocations []relocationYAML `yaml:"relocations"`
	Values      []valueYAML      `yaml:"values"`
}

type relocationYAML struct {
	Addr   uint32 `yaml:"addr"`
	To     string `yaml:"to"`
	Offset uint32 `yaml:"offset"`
}

type valueYAML struct {
	Addr uint32 `yaml:"addr"`
	End  uint32 `yaml:"end"`
	Why  string `yaml:"why"`
}

type functionYAML struct {
	Addr         uint32       `yaml:"addr"`
	Size         int          `yaml:"size"`
	Name         string       `yaml:"name"`
	Hash         string       `yaml:"hash"`
	Profile      string       `yaml:"profile"`
	Source       string       `yaml:"source"`
	SourceSymbol string       `yaml:"source_symbol"`
	Asm          string       `yaml:"asm"`
	Rodata       []rodataYAML `yaml:"rodata"`
}

type rodataYAML struct {
	Addr uint32 `yaml:"addr"`
	Size int    `yaml:"size"`
	Hash string `yaml:"hash"`
}

type symbolYAML struct {
	Addr uint32 `yaml:"addr"`
	Name string `yaml:"name"`
}

type overrideYAML struct {
	Function string `yaml:"function"`
	Name     string `yaml:"name"`
	Addr     uint32 `yaml:"addr"`
}

type libraryYAML struct {
	ID      string `yaml:"id"`
	Addr    uint32 `yaml:"addr"`
	End     uint32 `yaml:"end"`
	Kind    string `yaml:"kind"`
	Library string `yaml:"library"`
}

type regionYAML struct {
	Addr uint32 `yaml:"addr"`
	End  uint32 `yaml:"end"`
	Kind string `yaml:"kind"`
	Why  string `yaml:"why"`
}

type unitYAML struct {
	Addr    uint32 `yaml:"addr"`
	End     uint32 `yaml:"end"`
	Name    string `yaml:"name"`
	Profile string `yaml:"profile"`
}

func parseModuleConfig(data []byte) (*moduleSpec, error) {
	decoder := yaml.NewDecoder(bytes.NewReader(data))
	decoder.KnownFields(true)
	var file moduleFileYAML
	if err := decoder.Decode(&file); err != nil {
		return nil, err
	}
	module := &moduleSpec{
		Comment: leadingComment(data), ID: file.Module, File: file.File,
		Size: file.Size, SHA256: file.SHA256, Load: file.Load, SourceDir: file.SourceDir,
		Profile: file.Profile, Links: file.Links,
	}
	for _, item := range file.Libraries {
		module.Libraries = append(module.Libraries, librarySpec(item))
	}
	for _, item := range file.Functions {
		function := functionSpec{
			Addr: item.Addr, Size: item.Size, Name: item.Name, Hash: item.Hash,
			Profile: item.Profile, Source: item.Source, SourceSymbol: item.SourceSymbol, Asm: item.Asm,
		}
		for _, rodata := range item.Rodata {
			function.Rodata = append(function.Rodata, rodataSpec(rodata))
		}
		module.Functions = append(module.Functions, function)
	}
	for _, item := range file.Data {
		module.Data = append(module.Data, symbolSpec(item))
	}
	for _, item := range file.Imports {
		module.Imports = append(module.Imports, symbolSpec(item))
	}
	for _, item := range file.Overrides {
		module.Overrides = append(module.Overrides, overrideSpec(item))
	}
	for _, item := range file.Regions {
		module.Regions = append(module.Regions, regionSpec(item))
	}
	for _, item := range file.Units {
		module.Units = append(module.Units, unitSpec(item))
	}
	for _, item := range file.Relocations {
		module.Relocations = append(module.Relocations, relocationSpec(item))
	}
	for _, item := range file.Values {
		module.Values = append(module.Values, valueSpec(item))
	}
	return module, nil
}

func leadingComment(data []byte) string {
	var lines []string
	for _, line := range strings.Split(string(data), "\n") {
		if !strings.HasPrefix(line, "#") {
			break
		}
		lines = append(lines, strings.TrimRight(line, " \t\r"))
	}
	return strings.Join(lines, "\n")
}

// normalize sorts every list and drops values that restate a default, which is
// exactly the canonical form renderModuleConfig writes.
func (m *moduleSpec) normalize() {
	for index := range m.Functions {
		function := &m.Functions[index]
		if function.Profile == m.Profile {
			function.Profile = ""
		}
		if function.SourceSymbol == function.Name {
			function.SourceSymbol = ""
		}
		if function.Source == m.defaultSource(function) {
			function.Source = ""
		}
		sort.SliceStable(function.Rodata, func(i, j int) bool { return function.Rodata[i].Addr < function.Rodata[j].Addr })
	}
	sort.SliceStable(m.Functions, func(i, j int) bool { return m.Functions[i].Addr < m.Functions[j].Addr })
	sortSymbols(m.Data)
	sortSymbols(m.Imports)
	sort.SliceStable(m.Overrides, func(i, j int) bool {
		if m.Overrides[i].Function != m.Overrides[j].Function {
			return m.Overrides[i].Function < m.Overrides[j].Function
		}
		return m.Overrides[i].Name < m.Overrides[j].Name
	})
	sort.SliceStable(m.Libraries, func(i, j int) bool { return m.Libraries[i].Addr < m.Libraries[j].Addr })
	for index := range m.Units {
		if m.Units[index].Profile == m.Profile {
			m.Units[index].Profile = ""
		}
	}
	sort.SliceStable(m.Units, func(i, j int) bool { return m.Units[i].Addr < m.Units[j].Addr })
	sort.SliceStable(m.Regions, func(i, j int) bool {
		if m.Regions[i].Addr != m.Regions[j].Addr {
			return m.Regions[i].Addr < m.Regions[j].Addr
		}
		return m.Regions[i].End > m.Regions[j].End
	})
	sort.SliceStable(m.Relocations, func(i, j int) bool { return m.Relocations[i].Addr < m.Relocations[j].Addr })
	for index := range m.Values {
		if m.Values[index].End == m.Values[index].Addr+4 {
			m.Values[index].End = 0
		}
	}
	sort.SliceStable(m.Values, func(i, j int) bool { return m.Values[i].Addr < m.Values[j].Addr })
}

func sortSymbols(symbols []symbolSpec) {
	sort.SliceStable(symbols, func(i, j int) bool {
		if symbols[i].Addr != symbols[j].Addr {
			return symbols[i].Addr < symbols[j].Addr
		}
		return symbols[i].Name < symbols[j].Name
	})
}

// renderModuleConfig writes the canonical form: fixed key order, one flow
// mapping per list row, unquoted hex addresses and decimal sizes.
func renderModuleConfig(m *moduleSpec) []byte {
	m.normalize()
	var out bytes.Buffer
	if m.Comment != "" {
		out.WriteString(m.Comment + "\n")
	}
	fmt.Fprintf(&out, "module: %s\nfile: %s\nsize: %d\nsha256: %s\nload: 0x%08x\nsource_dir: %s\nprofile: %s\n",
		yamlText(m.ID), yamlText(m.File), m.Size, yamlText(m.SHA256), m.Load, yamlText(m.SourceDir), yamlText(m.Profile))
	if len(m.Links) > 0 {
		links := make([]string, len(m.Links))
		for index, link := range m.Links {
			links[index] = yamlText(link)
		}
		fmt.Fprintf(&out, "links: [%s]\n", strings.Join(links, ", "))
	}
	section := func(name string, count int, row func(int) string) {
		if count == 0 {
			return
		}
		fmt.Fprintf(&out, "\n%s:\n", name)
		for index := range count {
			fmt.Fprintf(&out, "  - {%s}\n", row(index))
		}
	}
	section("libraries", len(m.Libraries), func(index int) string {
		item := m.Libraries[index]
		return fmt.Sprintf("id: %s, addr: 0x%08x, end: 0x%08x, kind: %s, library: %s", yamlText(item.ID), item.Addr, item.End, yamlText(item.Kind), yamlText(item.Library))
	})
	section("units", len(m.Units), func(index int) string {
		item := m.Units[index]
		text := fmt.Sprintf("addr: 0x%08x, end: 0x%08x, name: %s", item.Addr, item.End, yamlText(item.Name))
		if item.Profile != "" {
			text += ", profile: " + yamlText(item.Profile)
		}
		return text
	})
	section("functions", len(m.Functions), func(index int) string {
		item := m.Functions[index]
		fields := []string{fmt.Sprintf("addr: 0x%08x", item.Addr), fmt.Sprintf("size: %d", item.Size), "name: " + yamlText(item.Name)}
		if item.Hash != "" {
			fields = append(fields, "hash: "+yamlText(item.Hash))
		}
		if item.Profile != "" {
			fields = append(fields, "profile: "+yamlText(item.Profile))
		}
		if item.Source != "" {
			fields = append(fields, "source: "+yamlText(item.Source))
		}
		if item.SourceSymbol != "" {
			fields = append(fields, "source_symbol: "+yamlText(item.SourceSymbol))
		}
		if item.Asm != "" {
			fields = append(fields, "asm: "+yamlText(item.Asm))
		}
		if len(item.Rodata) > 0 {
			ranges := make([]string, len(item.Rodata))
			for rodataIndex, rodata := range item.Rodata {
				text := fmt.Sprintf("addr: 0x%08x, size: %d", rodata.Addr, rodata.Size)
				if rodata.Hash != "" {
					text += ", hash: " + yamlText(rodata.Hash)
				}
				ranges[rodataIndex] = "{" + text + "}"
			}
			fields = append(fields, "rodata: ["+strings.Join(ranges, ", ")+"]")
		}
		return strings.Join(fields, ", ")
	})
	symbolRow := func(symbols []symbolSpec) func(int) string {
		return func(index int) string {
			return fmt.Sprintf("addr: 0x%08x, name: %s", symbols[index].Addr, yamlText(symbols[index].Name))
		}
	}
	section("data", len(m.Data), symbolRow(m.Data))
	section("imports", len(m.Imports), symbolRow(m.Imports))
	section("overrides", len(m.Overrides), func(index int) string {
		item := m.Overrides[index]
		return fmt.Sprintf("function: %s, name: %s, addr: 0x%08x", yamlText(item.Function), yamlText(item.Name), item.Addr)
	})
	section("regions", len(m.Regions), func(index int) string {
		item := m.Regions[index]
		return fmt.Sprintf("addr: 0x%08x, end: 0x%08x, kind: %s, why: %s", item.Addr, item.End, yamlText(item.Kind), yamlText(item.Why))
	})
	section("relocations", len(m.Relocations), func(index int) string {
		item := m.Relocations[index]
		text := fmt.Sprintf("addr: 0x%08x, to: %s", item.Addr, yamlText(item.To))
		if item.Offset != 0 {
			text += fmt.Sprintf(", offset: 0x%x", item.Offset)
		}
		return text
	})
	section("values", len(m.Values), func(index int) string {
		item := m.Values[index]
		text := fmt.Sprintf("addr: 0x%08x", item.Addr)
		if item.End != 0 {
			text += fmt.Sprintf(", end: 0x%08x", item.End)
		}
		return text + ", why: " + yamlText(item.Why)
	})
	return out.Bytes()
}

var plainYAMLText = regexp.MustCompile(`^[A-Za-z0-9_][A-Za-z0-9_./-]*$`)

// yamlText spells a string as a plain scalar when that reads back as the same
// string in flow context, and as a double-quoted scalar otherwise.
func yamlText(value string) string {
	if plainYAMLText.MatchString(value) {
		var decoded any
		if yaml.Unmarshal([]byte(value), &decoded) == nil {
			if text, ok := decoded.(string); ok && text == value {
				return value
			}
		}
	}
	var quoted bytes.Buffer
	encoder := json.NewEncoder(&quoted)
	encoder.SetEscapeHTML(false)
	_ = encoder.Encode(value)
	return strings.TrimSuffix(quoted.String(), "\n")
}

// renderConfigFiles renders every module file canonically (modules ordered by
// id, one YAML document each) and the disc layout.
func renderConfigFiles(config *projectConfig) map[string][]byte {
	grouped := make(map[string][]*moduleSpec)
	for _, m := range config.Modules {
		grouped[moduleConfigPath(m.ID)] = append(grouped[moduleConfigPath(m.ID)], m)
	}
	files := make(map[string][]byte, len(grouped)+1)
	if config.Disc != nil {
		files[discLayoutPath] = renderDiscLayout(config.Disc)
	}
	for path, modules := range grouped {
		sort.Slice(modules, func(i, j int) bool { return modules[i].ID < modules[j].ID })
		var out bytes.Buffer
		for index, m := range modules {
			if index > 0 {
				out.WriteString("---\n")
			}
			out.Write(renderModuleConfig(m))
		}
		files[path] = out.Bytes()
	}
	return files
}

// writeProjectConfig writes every module file whose canonical form differs
// from the disk and removes module files that no longer hold a module.
func (p project) writeProjectConfig(config *projectConfig) (int, error) {
	changed := 0
	rendered := renderConfigFiles(config)
	for path, data := range rendered {
		full := filepath.Join(p.root, filepath.FromSlash(path))
		if existing, err := os.ReadFile(full); err == nil && bytes.Equal(existing, data) {
			continue
		}
		if err := os.MkdirAll(filepath.Dir(full), 0o755); err != nil {
			return changed, err
		}
		if err := atomicWrite(full, data); err != nil {
			return changed, err
		}
		changed++
	}
	for path := range config.files {
		if _, ok := rendered[path]; !ok {
			if err := os.Remove(filepath.Join(p.root, filepath.FromSlash(path))); err != nil {
				return changed, err
			}
			changed++
		}
	}
	return changed, nil
}

// configFormat rewrites every module file in canonical form.
func (p project) configFormat() error {
	return p.withTargetLock(func() error {
		config, err := p.loadProjectConfig()
		if err != nil {
			return err
		}
		changed, err := p.writeProjectConfig(config)
		if err != nil {
			return err
		}
		fmt.Printf("config-fmt: %d of %d files rewritten\n", changed, len(config.files))
		return nil
	})
}

func validHash(value string) bool {
	if len(value) != hashDigits {
		return false
	}
	_, err := strconv.ParseUint(value, 16, 64)
	return err == nil && strings.ToLower(value) == value
}

// validateProjectConfig checks everything that can be checked without the
// disc image: canonical form, ordering, ranges, sources, hashes, links.
func (p project) validateProjectConfig(config *projectConfig) error {
	var problems []string
	fail := func(format string, args ...any) { problems = append(problems, fmt.Sprintf(format, args...)) }
	seenFiles := make(map[string]string)
	sources := make(map[string]bool)
	sourceSymbols := make(map[string]string)
	checkedDefinitions := make(map[string]bool)
	assemblyEntries, err := quirksAssemblyEntries(p.root)
	if err != nil {
		return err
	}
	resolver := newSymbolResolver(config)
	rendered := renderConfigFiles(config)
	paths := make([]string, 0, len(rendered))
	for path := range rendered {
		paths = append(paths, path)
	}
	sort.Strings(paths)
	for _, path := range paths {
		if !bytes.Equal(rendered[path], config.files[path]) {
			fail("%s is not in canonical form; run `make config-fmt`", path)
		}
	}
	if config.Disc == nil {
		fail("%s is missing", discLayoutPath)
	} else {
		validateDiscLayout(config.Disc, config, fail)
	}
	for _, m := range config.Modules {
		label := m.path + " " + m.ID
		if m.File == "" || seenFiles[m.File] != "" {
			fail("%s: missing or duplicate file %q", label, m.File)
		}
		seenFiles[m.File] = m.ID
		if m.Size <= 0 || !validSHA256(m.SHA256) || m.Load%4 != 0 || m.end() > 0x100000000 {
			fail("%s: invalid size, sha256 or load", label)
		}
		if m.SourceDir == "" || filepath.ToSlash(filepath.Clean(m.SourceDir)) != m.SourceDir || !strings.HasPrefix(m.SourceDir, "src/") {
			fail("%s: source_dir must be a clean path under src/", label)
		}
		if _, ok := compilerProfileNamed(m.Profile); !ok {
			fail("%s: unknown profile %q", label, m.Profile)
		}
		seenLinks := make(map[string]bool)
		for _, link := range m.Links {
			if _, ok := config.byID[link]; !ok || link == m.ID || link == mainModuleID || seenLinks[link] {
				fail("%s: invalid link %q (main is always searched last)", label, link)
			}
			seenLinks[link] = true
		}
		names := make(map[string]string)
		claim := func(name, what string) {
			if !validSymbol(name) {
				fail("%s: invalid %s name %q", label, what, name)
			} else if previous := names[name]; previous != "" {
				fail("%s: %s %s duplicates a %s name", label, what, name, previous)
			}
			names[name] = what
		}
		moduleSources := make(map[string]string)
		var previousEnd uint64
		for index := range m.Functions {
			f := &m.Functions[index]
			claim(f.Name, "function")
			where := fmt.Sprintf("%s: function %s", label, f.Name)
			if f.Addr%4 != 0 || f.Size <= 0 || f.Size%4 != 0 || !m.contains(f.Addr, f.Size) {
				fail("%s has an invalid or out-of-image range", where)
			}
			if uint64(f.Addr) < previousEnd {
				fail("%s overlaps the previous function", where)
			}
			previousEnd = uint64(f.Addr) + uint64(f.Size)
			if !validHash(f.Hash) {
				fail("%s has no valid hash; run `make checksums`", where)
			}
			if _, ok := compilerProfileNamed(m.profileOf(f)); !ok {
				fail("%s uses unknown profile %q", where, m.profileOf(f))
			}
			source := m.sourceOf(f)
			if f.Asm != "" {
				if strings.TrimSpace(f.Asm) == "" {
					fail("%s: asm needs the reason the routine is assembly", where)
				}
				if source != m.defaultSource(f) {
					fail("%s: assembly is its own source named after it, %s", where, m.defaultSource(f))
				}
				if !assemblyEntries[f.Name] {
					fail("%s: QUIRKS.md has no %q entry for it (its Port debt)", where, quirksAssemblySection)
				}
			} else if filepath.Ext(source) != ".c" {
				fail("%s: C function requires a .c source (set asm for assembly)", where)
			}
			_, sourceErr := projectPath(p.root, source, "source")
			if sourceErr != nil {
				fail("%s: %v", where, sourceErr)
			} else if f.Asm != "" {
				if err := validateAssemblySource(p.root, source, f); err != nil {
					fail("%s: %v", where, err)
				}
			}
			sourceSymbol := f.sourceSymbol()
			if !validSymbol(sourceSymbol) {
				fail("%s: invalid source_symbol %q", where, sourceSymbol)
			}
			if previous := sourceSymbols[source]; previous != "" && previous != sourceSymbol {
				fail("%s: source %s has conflicting definition symbols %s and %s", where, source, previous, sourceSymbol)
			}
			sourceSymbols[source] = sourceSymbol
			if f.SourceSymbol != "" && f.Asm != "" {
				fail("%s: source_symbol requires a C source", where)
			}
			if (f.SourceSymbol != "" || moduleSources[source] != "") && f.Asm == "" && sourceErr == nil && !checkedDefinitions[source] {
				checkedDefinitions[source] = true
				if err := validateSourceDefinition(p.root, source, sourceSymbol); err != nil {
					fail("%s: %v", where, err)
				}
			}
			moduleSources[source] = f.Name
			sources[source] = true
			for _, rodata := range f.Rodata {
				if rodata.Size <= 0 || !m.contains(rodata.Addr, rodata.Size) {
					fail("%s: rodata 0x%08x lies outside the module image", where, rodata.Addr)
				}
				if !validHash(rodata.Hash) {
					fail("%s: rodata 0x%08x has no valid hash; run `make checksums`", where, rodata.Addr)
				}
			}
		}
		for _, symbol := range m.Data {
			claim(symbol.Name, "data")
		}
		for _, symbol := range m.Imports {
			claim(symbol.Name, "import")
		}
		for _, override := range m.Overrides {
			f := m.function(override.Function)
			if f != nil && f.SourceSymbol != "" && override.Name == f.sourceSymbol() && override.Addr != f.Addr {
				fail("%s: override %s/%s conflicts with its source definition", label, override.Function, override.Name)
			}
			if f == nil || !validSymbol(override.Name) {
				fail("%s: override %s/%s names no function of this module", label, override.Function, override.Name)
			}
		}
		var libraryEnd uint32
		seenLibraries := make(map[string]bool)
		for _, library := range m.Libraries {
			if !validSymbol(library.ID) || seenLibraries[library.ID] || !libraryKinds[library.Kind] || library.Library == "" {
				fail("%s: library %q has an invalid id, kind or name", label, library.ID)
			}
			seenLibraries[library.ID] = true
			if library.Addr%4 != 0 || library.End%4 != 0 || library.End <= library.Addr || library.Addr < libraryEnd || !m.contains(library.Addr, int(library.End-library.Addr)) {
				fail("%s: library %s range is invalid, overlapping or outside the image", label, library.ID)
			}
			libraryEnd = library.End
		}
		var unitEnd uint32
		seenUnits := make(map[string]bool)
		for _, unit := range m.Units {
			where := fmt.Sprintf("%s: unit %s", label, unit.Name)
			if !validSymbol(unit.Name) || seenUnits[unit.Name] {
				fail("%s has an invalid or duplicate name", where)
			}
			seenUnits[unit.Name] = true
			if _, ok := compilerProfileNamed(m.unitProfileName(unit)); !ok {
				fail("%s uses unknown profile %q", where, unit.Profile)
			}
			if unit.End <= unit.Addr || unit.Addr < unitEnd || !m.contains(unit.Addr, int(unit.End-unit.Addr)) {
				fail("%s range is invalid, overlapping or outside the image", where)
				continue
			}
			unitEnd = unit.End
			// The members tile the range: C functions, each one starting where
			// the previous one ends.
			next := unit.Addr
			for _, f := range m.unitMembers(unit) {
				if f.Addr != next {
					break
				}
				if f.Asm != "" {
					fail("%s holds assembly %s", where, f.Name)
				}
				next = f.Addr + uint32(f.Size)
			}
			if next != unit.End {
				fail("%s has no function continuing it at 0x%08x", where, next)
			}
		}
		for _, region := range m.Regions {
			where := fmt.Sprintf("%s: region 0x%08x-0x%08x", label, region.Addr, region.End)
			if !regionKinds[region.Kind] || strings.TrimSpace(region.Why) == "" {
				fail("%s needs a known kind and a why", where)
			}
			if region.Addr%4 != 0 || region.End%4 != 0 || region.End <= region.Addr || !m.contains(region.Addr, int(region.End-region.Addr)) {
				fail("%s is invalid or outside the image", where)
			}
			if region.Kind == "blocked" || region.Kind == "handwritten" {
				covered := false
				for _, f := range m.Functions {
					if region.Addr >= f.Addr+uint32(f.Size) || f.Addr >= region.End {
						continue
					}
					covered = true
					switch {
					case f.Asm != "":
						fail("%s is %s, but %s is its source now; remove the region", where, region.Kind, f.Name)
					case region.Kind == "blocked":
						fail("%s is blocked but overlaps matching function %s; remove the region", where, f.Name)
					}
				}
				// A handwritten range inside a C function is that function's
				// inline assembly; any other code has a source.
				if !covered {
					fail("%s is %s code with no source; write it as a .s routine", where, region.Kind)
				}
			}
		}
		// Each data word is classified once, as a relocation or as a value.
		classified := make(map[uint32]string)
		for _, relocation := range m.Relocations {
			where := fmt.Sprintf("%s: relocation 0x%08x", label, relocation.Addr)
			if relocation.Addr%4 != 0 || !m.contains(relocation.Addr, 4) {
				fail("%s is not an aligned word of the file", where)
			} else if owner := m.codeAt(relocation.Addr, relocation.Addr+4); owner != "" {
				fail("%s lies in %s", where, owner)
			}
			if previous := classified[relocation.Addr]; previous != "" {
				fail("%s repeats a %s", where, previous)
			}
			classified[relocation.Addr] = "relocation"
			if _, ok := resolver.resolve(m, relocation.To); !ok {
				fail("%s: %s is no name this module resolves", where, relocation.To)
			}
		}
		for _, value := range m.Values {
			where := fmt.Sprintf("%s: value 0x%08x", label, value.Addr)
			end := value.end()
			if value.Addr%4 != 0 || end%4 != 0 || end <= value.Addr || !m.contains(value.Addr, int(end-value.Addr)) {
				fail("%s is not an aligned range of the file", where)
				continue
			}
			if owner := m.codeAt(value.Addr, end); owner != "" {
				fail("%s lies in %s", where, owner)
			}
			if strings.TrimSpace(value.Why) == "" {
				fail("%s needs a why", where)
			}
			for addr := value.Addr; addr < end; addr += 4 {
				if previous := classified[addr]; previous != "" {
					fail("%s overlaps a %s at 0x%08x", where, previous, addr)
					break
				}
				classified[addr] = "value"
			}
		}
	}
	if err := validateSourceInventory(p.root, "src", sources, "module configuration"); err != nil {
		fail("%v", err)
	}
	if len(problems) > 0 {
		if len(problems) > 40 {
			problems = append(problems[:40], fmt.Sprintf("... and %d more", len(problems)-40))
		}
		return errors.New("config check failed:\n  " + strings.Join(problems, "\n  "))
	}
	return nil
}
