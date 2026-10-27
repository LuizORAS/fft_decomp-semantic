// ram_map.go loads, checks and canonically writes target/ram.yaml, the RAM
// windows the modules load into and the states that keep several of them
// resident at once, and generates include/fft/ram.h, the window sizes the
// code compiles in.
package main

import (
	"bytes"
	"fmt"
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strings"

	"gopkg.in/yaml.v3"
)

const ramMapPath = modulesConfigDir + "/ram.yaml"

// ramMap is target/ram.yaml. A window is the RAM from its modules' load
// address to where the next thing resident with them starts; a state lists
// the windows resident at one time.
type ramMap struct {
	Comment string
	Windows []ramWindow
	States  []ramState
}

type ramWindow struct {
	Name    string   `yaml:"name"`
	Addr    uint32   `yaml:"addr"`
	End     uint32   `yaml:"end"`
	Modules []string `yaml:"modules"` // module ids, or `event` and `effect` for every such module
	Why     string   `yaml:"why"`     // what starts at End
}

type ramState struct {
	Name    string   `yaml:"name"`
	Windows []string `yaml:"windows"`
}

type ramMapYAML struct {
	Windows []ramWindow `yaml:"windows"`
	States  []ramState  `yaml:"states"`
}

var ramNamePattern = regexp.MustCompile(`^[a-z][a-z0-9_]*$`)

func parseRAMMap(data []byte) (*ramMap, error) {
	decoder := yaml.NewDecoder(bytes.NewReader(data))
	decoder.KnownFields(true)
	var file ramMapYAML
	if err := decoder.Decode(&file); err != nil {
		return nil, err
	}
	return &ramMap{Comment: leadingComment(data), Windows: file.Windows, States: file.States}, nil
}

// renderRAMMap writes the canonical form: the windows in address order (the
// shorter first where two start together), the states as written.
func renderRAMMap(ram *ramMap) []byte {
	sort.SliceStable(ram.Windows, func(i, j int) bool {
		if ram.Windows[i].Addr != ram.Windows[j].Addr {
			return ram.Windows[i].Addr < ram.Windows[j].Addr
		}
		return ram.Windows[i].End < ram.Windows[j].End
	})
	var out bytes.Buffer
	if ram.Comment != "" {
		out.WriteString(ram.Comment + "\n")
	}
	list := func(names []string) string {
		quoted := make([]string, len(names))
		for index, name := range names {
			quoted[index] = yamlText(name)
		}
		return "[" + strings.Join(quoted, ", ") + "]"
	}
	out.WriteString("windows:\n")
	for _, window := range ram.Windows {
		fmt.Fprintf(&out, "  - {name: %s, addr: 0x%08x, end: 0x%08x, modules: %s, why: %s}\n",
			yamlText(window.Name), window.Addr, window.End, list(window.Modules), yamlText(window.Why))
	}
	if len(ram.States) > 0 {
		out.WriteString("\nstates:\n")
		for _, state := range ram.States {
			fmt.Fprintf(&out, "  - {name: %s, windows: %s}\n", yamlText(state.Name), list(state.Windows))
		}
	}
	return out.Bytes()
}

// moduleRAM returns the RAM a module's image takes: from its load address
// (past MAIN's PS-X EXE header, which stays on the disc) to its end.
func moduleRAM(m *moduleSpec) (uint32, uint32) {
	start := m.Load
	if m.ID == mainModuleID {
		start += psxEXEHeaderSize
	}
	return start, m.Load + uint32(m.Size)
}

// window returns the window a name gives, or nil.
func (ram *ramMap) window(name string) *ramWindow {
	for index := range ram.Windows {
		if ram.Windows[index].Name == name {
			return &ram.Windows[index]
		}
	}
	return nil
}

// moduleWindows maps each module id to its window, with the names that select
// no module.
func (ram *ramMap) moduleWindows(config *projectConfig) (map[string]*ramWindow, []string) {
	windows := make(map[string]*ramWindow)
	var unknown []string
	for index := range ram.Windows {
		window := &ram.Windows[index]
		for _, name := range window.Modules {
			modules, err := resolveModuleSelection(config, name)
			if err != nil {
				unknown = append(unknown, name)
				continue
			}
			for _, m := range modules {
				if windows[m.ID] == nil {
					windows[m.ID] = window
				} else if windows[m.ID] != window {
					windows[m.ID] = nil // in two windows: validateRAMMap reports it
				}
			}
		}
	}
	return windows, unknown
}

// validateRAMMap checks the windows (unique names, a start before the end, a
// why), that each module loads at the start of exactly one window and ends
// inside it, and the states (known windows, none overlapping another, every
// window in one state at least).
func validateRAMMap(ram *ramMap, config *projectConfig, fail func(string, ...any)) {
	label := ramMapPath
	names := make(map[string]bool)
	for _, window := range ram.Windows {
		where := fmt.Sprintf("%s: window %s", label, window.Name)
		switch {
		case !ramNamePattern.MatchString(window.Name) || names[window.Name]:
			fail("%s needs a new lowercase name", where)
		case window.End <= window.Addr:
			fail("%s ends at 0x%08x, before it starts", where, window.End)
		case strings.TrimSpace(window.Why) == "" || len(window.Modules) == 0:
			fail("%s needs its modules and a why", where)
		}
		names[window.Name] = true
	}
	windows, unknown := ram.moduleWindows(config)
	for _, name := range unknown {
		fail("%s: %s is no module", label, name)
	}
	for _, m := range config.Modules {
		window, listed := windows[m.ID]
		start, end := moduleRAM(m)
		switch {
		case !listed:
			fail("%s: module %s is in no window", label, m.ID)
		case window == nil:
			fail("%s: module %s is in two windows", label, m.ID)
		case start != window.Addr:
			fail("%s: module %s loads at 0x%08x, but window %s starts at 0x%08x", label, m.ID, start, window.Name, window.Addr)
		case end > window.End:
			fail("%s: module %s ends at 0x%08x, past window %s's end 0x%08x", label, m.ID, end, window.Name, window.End)
		}
	}
	inState := make(map[string]bool)
	for _, state := range ram.States {
		where := fmt.Sprintf("%s: state %s", label, state.Name)
		var members []*ramWindow
		for _, name := range state.Windows {
			window := ram.window(name)
			if window == nil {
				fail("%s: %s is no window", where, name)
				continue
			}
			for _, other := range members {
				if window.Addr < other.End && other.Addr < window.End {
					fail("%s: windows %s and %s overlap", where, other.Name, window.Name)
				}
			}
			members = append(members, window)
			inState[name] = true
		}
	}
	for _, window := range ram.Windows {
		if !inState[window.Name] {
			fail("%s: window %s is in no state", label, window.Name)
		}
	}
}

// ramHeaderPath is the header of the RAM map's facts the code compiles in.
const ramHeaderPath = "include/fft/ram.h"

const ramWindowBytesPrefix = "RAM_WINDOW_BYTES_"

// ramHeader writes the header for the names the code uses:
// RAM_WINDOW_BYTES_<WINDOW> is the window's size in bytes, in window order.
// It returns nil when the code uses no name, and the names that spell no
// window.
func ramHeader(ram *ramMap, names map[string]bool) ([]byte, []string) {
	if len(names) == 0 {
		return nil, nil
	}
	var sizes []string
	known := make(map[string]bool)
	for _, window := range ram.Windows {
		name := ramWindowBytesPrefix + strings.ToUpper(window.Name)
		if names[name] {
			sizes = append(sizes, fmt.Sprintf("    %s = 0x%x,\n", name, window.End-window.Addr))
			known[name] = true
		}
	}
	var unknown []string
	for name := range names {
		if !known[name] {
			unknown = append(unknown, name)
		}
	}
	sort.Strings(unknown)
	var out bytes.Buffer
	out.WriteString(`#ifndef RAM_H
#define RAM_H

/* The RAM windows the code sizes reads and reservations by, generated from target/ram.yaml by
 * ` + "`make config-fmt`" + `: edit the map, not this file. */

/* ram */
/* The bytes from a window's start to where the next thing resident with it starts. */
`)
	if len(sizes) > 0 {
		out.WriteString("\ntypedef enum ram_window_bytes {\n" + strings.Join(sizes, "") + "} ram_window_bytes_e;\n")
	}
	out.WriteString("\n#endif\n")
	return out.Bytes(), unknown
}

// validateRAMHeader fails when the header differs from what the map gives for
// the names the code uses, or when a name spells no window.
func (p project) validateRAMHeader(config *projectConfig, fail func(string, ...any)) error {
	names, err := codeNamesWithPrefix(p.root, ramWindowBytesPrefix, ramHeaderPath)
	if err != nil {
		return err
	}
	want, unknown := ramHeader(config.RAM, names)
	for _, name := range unknown {
		fail("%s: %s names no window in %s", ramHeaderPath, name, ramMapPath)
	}
	if want == nil {
		return nil
	}
	if got, err := os.ReadFile(filepath.Join(p.root, filepath.FromSlash(ramHeaderPath))); err != nil || !bytes.Equal(got, want) {
		fail("%s is not what %s gives; run `make config-fmt`", ramHeaderPath, ramMapPath)
	}
	return nil
}

// writeRAMHeader writes the header when it differs, and reports whether it
// did.
func (p project) writeRAMHeader(config *projectConfig) (bool, error) {
	if config.RAM == nil {
		return false, nil
	}
	names, err := codeNamesWithPrefix(p.root, ramWindowBytesPrefix, ramHeaderPath)
	if err != nil {
		return false, err
	}
	want, _ := ramHeader(config.RAM, names)
	full := filepath.Join(p.root, filepath.FromSlash(ramHeaderPath))
	if got, err := os.ReadFile(full); want == nil || err == nil && bytes.Equal(got, want) {
		return false, nil
	}
	if err := os.MkdirAll(filepath.Dir(full), 0o755); err != nil {
		return false, err
	}
	return true, atomicWrite(full, want)
}

// ramRoom lists, for each window that holds one of the modules, the bytes
// left between its largest module's end and the window's end.
func ramRoom(ram *ramMap, config *projectConfig, modules []*moduleSpec) []string {
	windows, _ := ram.moduleWindows(config)
	type room struct {
		window *ramWindow
		module *moduleSpec
		end    uint32
	}
	rooms := make(map[*ramWindow]*room)
	for _, m := range modules {
		window := windows[m.ID]
		if window == nil {
			continue
		}
		_, end := moduleRAM(m)
		if current := rooms[window]; current == nil || end > current.end {
			rooms[window] = &room{window, m, end}
		}
	}
	var lines []string
	for index := range ram.Windows {
		if r := rooms[&ram.Windows[index]]; r != nil {
			lines = append(lines, fmt.Sprintf("  %-18s 0x%08x-0x%08x %7d bytes left after %s", r.window.Name, r.window.Addr, r.window.End, r.window.End-r.end, r.module.ID))
		}
	}
	return lines
}
