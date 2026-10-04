// codebase_map_extras.go writes the by-products of `make map` that are not
// vault pages: PCSX-Redux symbol maps in build/symbols/, and the page that
// translates upstream names into ours by address.
package main

import (
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"
)

const mapUpstreamPage = "Upstream renames"

// mapSymbolScenes are the module sets resident together; overlays share
// addresses, so each PCSX-Redux map covers one scene.
var mapSymbolScenes = []struct {
	file    string
	modules []string
}{
	{"fft_battle.map", []string{"main", "battle"}},
	{"fft_world.map", []string{"main", "wldcore", "world"}},
	{"fft_opening.map", []string{"main", "opening"}},
}

// writeSymbolMaps writes "address name" maps for PCSX-Redux. At an address
// with several names the function, then the first data or import row, wins.
func writeSymbolMaps(root string, config *projectConfig) (int, error) {
	out := filepath.Join(root, "build", "symbols")
	if err := os.MkdirAll(out, 0o755); err != nil {
		return 0, err
	}
	for _, scene := range mapSymbolScenes {
		names := map[uint32]string{}
		for _, id := range scene.modules {
			m, ok := config.module(id)
			if !ok {
				continue
			}
			add := func(addr uint32, name string) {
				if _, seen := names[addr]; !seen && addr >= 0x80000000 {
					names[addr] = name
				}
			}
			for _, f := range m.Functions {
				add(f.Addr, f.Name)
			}
			for _, data := range m.Data {
				add(data.Addr, data.Name)
			}
			for _, imported := range m.Imports {
				add(imported.Addr, imported.Name)
			}
		}
		addresses := make([]uint32, 0, len(names))
		for addr := range names {
			addresses = append(addresses, addr)
		}
		sort.Slice(addresses, func(i, j int) bool { return addresses[i] < addresses[j] })
		var b strings.Builder
		for _, addr := range addresses {
			fmt.Fprintf(&b, "%08x %s\n", addr, names[addr])
		}
		if err := atomicWrite(filepath.Join(out, scene.file), []byte(b.String())); err != nil {
			return 0, err
		}
	}
	return len(mapSymbolScenes), nil
}

type mapRename struct {
	module   string
	addr     uint32
	upstream string
	ours     string
}

// upstreamRenames reads the upstream target/ files the Makefile exported to
// build/upstream/target and pairs each upstream function or data name with
// ours at the same module and address. ok is false when there is no export.
func upstreamRenames(root string, ours *projectConfig) (renames []mapRename, ok bool, err error) {
	upstreamRoot := filepath.Join(root, "build", "upstream")
	if _, statErr := os.Stat(filepath.Join(upstreamRoot, modulesConfigDir, mainModuleID+".yaml")); statErr != nil {
		return nil, false, nil
	}
	theirs, err := project{root: upstreamRoot}.loadProjectConfig()
	if err != nil {
		return nil, true, err
	}
	for _, m := range theirs.Modules {
		mine, found := ours.module(m.ID)
		if !found {
			continue
		}
		names := map[uint32][]string{}
		for _, f := range mine.Functions {
			names[f.Addr] = append(names[f.Addr], f.Name)
		}
		for _, data := range mine.Data {
			names[data.Addr] = append(names[data.Addr], data.Name)
		}
		check := func(addr uint32, name string) {
			candidates := names[addr]
			if len(candidates) == 0 {
				return
			}
			for _, candidate := range candidates {
				if candidate == name {
					return
				}
			}
			renames = append(renames, mapRename{module: m.ID, addr: addr, upstream: name, ours: candidates[0]})
		}
		for _, f := range m.Functions {
			check(f.Addr, f.Name)
		}
		for _, data := range m.Data {
			check(data.Addr, data.Name)
		}
	}
	sort.SliceStable(renames, func(i, j int) bool {
		if renames[i].module != renames[j].module {
			return renames[i].module < renames[j].module
		}
		if renames[i].addr != renames[j].addr {
			return renames[i].addr < renames[j].addr
		}
		return renames[i].upstream < renames[j].upstream
	})
	return renames, true, nil
}

func (vault *mapVault) upstreamPage(renames []mapRename, available bool, readErr error) string {
	var b strings.Builder
	b.WriteString(frontmatter(mapProperty{"type", "report"}, mapProperty{"renames", len(renames)}))
	fmt.Fprintf(&b, "# %s\n\n", mapUpstreamPage)
	b.WriteString("Upstream function and data names that differ from ours at the same module and address: the table to translate an upstream commit into this tree. ")
	b.WriteString("`make map` exports upstream's `target/` from the `upstream/master` ref (`MAP_UPSTREAM=` picks another) before it runs.\n\n")
	switch {
	case readErr != nil:
		fmt.Fprintf(&b, "The upstream `target/` files could not be read: %v\n", readErr)
		return b.String()
	case !available:
		b.WriteString("No upstream export was found (no `upstream/master` ref in this clone).\n")
		return b.String()
	case len(renames) == 0:
		b.WriteString("No names differ.\n")
		return b.String()
	}
	fmt.Fprintf(&b, "%d names differ.\n\n| Module | Address | Upstream | Ours |\n|---|---|---|---|\n", len(renames))
	for _, rename := range renames {
		ours := "`" + rename.ours + "`"
		if _, isFunction := vault.index.functions[rename.ours]; isFunction {
			ours = vault.functionLink(rename.ours)
		} else if _, isGlobal := vault.index.globals[rename.ours]; isGlobal {
			ours = vault.globalLink(rename.ours)
		}
		b.WriteString(tableRow(rename.module, "`"+hex32(rename.addr)+"`", "`"+rename.upstream+"`", ours))
	}
	return b.String()
}
