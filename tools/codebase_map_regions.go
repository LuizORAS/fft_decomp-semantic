// codebase_map_regions.go shows the code the build keeps as original bytes:
// the target/ `regions` (handwritten assembly, blocked C, data, padding). A
// handwritten or blocked routine with no C function gets its own page, named
// by the data row at its start, so its callers can link to it.
package main

import (
	"fmt"
	"sort"
	"strings"
)

type mapRegion struct {
	module    string
	addr, end uint32
	kind, why string
	name      string // data name at the start, if any
	function  string // function row at the start, if any (C wrapping the bytes)

	calledBy, referencedBy []string
}

// key is the page key of a routine region, "" when it has no page of its own.
func (r *mapRegion) key() string {
	if r.function != "" || r.name == "" || (r.kind != "handwritten" && r.kind != "blocked") {
		return ""
	}
	return "region:" + r.name
}

// scanRegions indexes the non-interior regions and the data names that start
// or sit inside them.
func (index *codebaseIndex) scanRegions() {
	for _, m := range index.config.Modules {
		dataAt := map[uint32]string{}
		for _, data := range m.Data {
			if _, seen := dataAt[data.Addr]; !seen {
				dataAt[data.Addr] = data.Name
			}
		}
		functionAt := map[uint32]string{}
		for _, f := range m.Functions {
			functionAt[f.Addr] = f.Name
		}
		for _, spec := range m.Regions {
			if spec.Kind == "interior" {
				continue
			}
			region := &mapRegion{module: m.ID, addr: spec.Addr, end: spec.End, kind: spec.Kind, why: spec.Why,
				name: dataAt[spec.Addr], function: functionAt[spec.Addr]}
			index.regions = append(index.regions, region)
			if region.key() == "" {
				continue
			}
			for _, data := range m.Data {
				if data.Addr >= region.addr && data.Addr < region.end {
					if _, seen := index.regionByName[data.Name]; !seen {
						index.regionByName[data.Name] = region
					}
				}
			}
		}
	}
	sort.SliceStable(index.regions, func(i, j int) bool {
		if index.regions[i].module != index.regions[j].module {
			return index.regions[i].module < index.regions[j].module
		}
		return index.regions[i].addr < index.regions[j].addr
	})
}

// regionsOf lists a module's regions.
func (index *codebaseIndex) regionsOf(module string) []*mapRegion {
	var regions []*mapRegion
	for _, region := range index.regions {
		if region.module == module {
			regions = append(regions, region)
		}
	}
	return regions
}

// codeLink links a called or referenced name: a function page, or the page
// of the original-bytes routine it names.
func (vault *mapVault) codeLink(name string) string {
	if region := vault.index.regionByName[name]; region != nil {
		return vault.link(region.key(), name)
	}
	return vault.functionLink(name)
}

func (vault *mapVault) regionPage(region *mapRegion) string {
	var b strings.Builder
	b.WriteString(frontmatter(
		mapProperty{"type", "region"},
		mapProperty{"kind", region.kind},
		mapProperty{"module", region.module},
		mapProperty{"address", hex32(region.addr)},
		mapProperty{"size", int(region.end - region.addr)},
		mapProperty{"called_by", len(region.calledBy)},
	))
	fmt.Fprintf(&b, "# %s\n\n", region.name)
	fmt.Fprintf(&b, "Original %s code kept as bytes (`target/` region `%s`): %s.\n\n", region.kind, region.kind, region.why)
	fmt.Fprintf(&b, "- Module %s, `%s`–`%s` (%d bytes)\n", vault.moduleLink(region.module), hex32(region.addr), hex32(region.end), region.end-region.addr)
	var others []string
	for name, other := range vault.index.regionByName {
		if other == region && name != region.name {
			others = append(others, name)
		}
	}
	sort.Strings(others)
	if len(others) > 0 {
		fmt.Fprintf(&b, "- Also named inside it: `%s`\n", strings.Join(others, "`, `"))
	}
	if len(region.calledBy) > 0 {
		fmt.Fprintf(&b, "\n## Called by (%d)\n\n%s\n", len(region.calledBy), joinLinks(region.calledBy, vault.functionLink))
	}
	if len(region.referencedBy) > 0 {
		fmt.Fprintf(&b, "\n## Address used by (%d)\n\n%s\n", len(region.referencedBy), joinLinks(region.referencedBy, vault.functionLink))
	}
	b.WriteString(vault.quirksSection(vault.quirksFor(region.key())))
	return b.String()
}

// regionsSection renders a module's regions for its page.
func (vault *mapVault) regionsSection(module string) string {
	regions := vault.index.regionsOf(module)
	if len(regions) == 0 {
		return ""
	}
	var b strings.Builder
	b.WriteString("\n## Kept as original bytes\n\n| Range | Kind | Name | Why |\n|---|---|---|---|\n")
	for _, region := range regions {
		name := ""
		switch {
		case region.function != "":
			name = vault.functionLink(region.function)
		case region.key() != "":
			name = vault.link(region.key(), region.name)
		case region.name != "":
			name = "`" + region.name + "`"
		}
		b.WriteString(tableRow("`"+hex32(region.addr)+"`–`"+hex32(region.end)+"`", region.kind, name, region.why))
	}
	return b.String()
}
