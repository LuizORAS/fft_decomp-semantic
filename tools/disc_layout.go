// disc_layout.go loads, checks and canonically writes target/disc.yaml, the
// disc's ISO 9660 file system without its bytes, and writes the mkpsxiso
// project that builds the disc from it with every entry forced to the sector
// the layout gives it.
package main

import (
	"bytes"
	"fmt"
	"path"
	"regexp"
	"sort"
	"strings"

	"gopkg.in/yaml.v3"
)

const discLayoutPath = modulesConfigDir + "/disc.yaml"

// A data file fills 2048-byte Mode 2 Form 1 sectors; a mixed file (an STR
// movie or XA audio) carries 2336 bytes a sector, subheaders included.
const (
	discDataSectorBytes  = 2048
	discMixedSectorBytes = 2336
)

// discLayout is target/disc.yaml: the volume, mkpsxiso's default attributes,
// the root directory and every entry in sector order.
type discLayout struct {
	Comment    string
	License    string // extracted file holding the 16 system-area sectors
	Volume     discVolume
	Attributes discAttributes
	Root       discRoot
	Entries    []discEntry
}

type discVolume struct {
	System           string `yaml:"system"`
	Application      string `yaml:"application"`
	Volume           string `yaml:"volume"`
	VolumeSet        string `yaml:"volume_set"`
	Publisher        string `yaml:"publisher"`
	DataPreparer     string `yaml:"data_preparer"`
	CreationDate     string `yaml:"creation_date"`
	ModificationDate string `yaml:"modification_date"`
}

// discAttributes are the attributes of every entry that does not set its own.
type discAttributes struct {
	GMTOffset int `yaml:"gmt_offs"`
	XAAttrib  int `yaml:"xa_attrib"`
	XAPerm    int `yaml:"xa_perm"`
	XAGID     int `yaml:"xa_gid"`
	XAUID     int `yaml:"xa_uid"`
	Hidden    int `yaml:"hidden"`
}

// discRoot is the root directory record; every directory has its XA
// attributes.
type discRoot struct {
	LBA    int `yaml:"lba"`
	XAPerm int `yaml:"xa_perm"`
	XAGID  int `yaml:"xa_gid"`
	XAUID  int `yaml:"xa_uid"`
	Hidden int `yaml:"hidden"`
}

// discEntry is one row starting at sector LBA: a directory record (Dir), a
// file (File) or a dummy run of Dummy empty sectors in Form.
type discEntry struct {
	LBA   int    `yaml:"lba"`
	Dir   string `yaml:"dir"`
	File  string `yaml:"file"`
	Size  *int   `yaml:"size"` // nil for a module's file, sized by its module
	Type  string `yaml:"type"` // "" for data, or "mixed"
	Date  string `yaml:"date"`
	Fixed string `yaml:"fixed"` // why a directory's files stay at their sectors
	Dummy int    `yaml:"dummy"`
	Form  int    `yaml:"form"`
}

type discLayoutYAML struct {
	License    string         `yaml:"license"`
	Volume     discVolume     `yaml:"volume"`
	Attributes discAttributes `yaml:"attributes"`
	Root       discRoot       `yaml:"root"`
	Entries    []discEntry    `yaml:"entries"`
}

var discDatePattern = regexp.MustCompile(`^[0-9]{14}$`)

func parseDiscLayout(data []byte) (*discLayout, error) {
	decoder := yaml.NewDecoder(bytes.NewReader(data))
	decoder.KnownFields(true)
	var file discLayoutYAML
	if err := decoder.Decode(&file); err != nil {
		return nil, err
	}
	return &discLayout{
		Comment: leadingComment(data), License: file.License, Volume: file.Volume,
		Attributes: file.Attributes, Root: file.Root, Entries: file.Entries,
	}, nil
}

// renderDiscLayout writes the canonical form: fixed key order and one flow
// mapping per entry, in sector order.
func renderDiscLayout(layout *discLayout) []byte {
	sort.SliceStable(layout.Entries, func(i, j int) bool { return layout.Entries[i].LBA < layout.Entries[j].LBA })
	var out bytes.Buffer
	if layout.Comment != "" {
		out.WriteString(layout.Comment + "\n")
	}
	v, a, r := layout.Volume, layout.Attributes, layout.Root
	fmt.Fprintf(&out, "license: %s\n", yamlText(layout.License))
	fmt.Fprintf(&out, "volume: {system: %s, application: %s, volume: %s, volume_set: %s, publisher: %s, data_preparer: %s, creation_date: %s, modification_date: %s}\n",
		yamlText(v.System), yamlText(v.Application), yamlText(v.Volume), yamlText(v.VolumeSet), yamlText(v.Publisher),
		yamlText(v.DataPreparer), yamlText(v.CreationDate), yamlText(v.ModificationDate))
	fmt.Fprintf(&out, "attributes: {gmt_offs: %d, xa_attrib: %d, xa_perm: %d, xa_gid: %d, xa_uid: %d, hidden: %d}\n",
		a.GMTOffset, a.XAAttrib, a.XAPerm, a.XAGID, a.XAUID, a.Hidden)
	fmt.Fprintf(&out, "root: {lba: %d, xa_perm: %d, xa_gid: %d, xa_uid: %d, hidden: %d}\n", r.LBA, r.XAPerm, r.XAGID, r.XAUID, r.Hidden)
	if len(layout.Entries) > 0 {
		out.WriteString("\nentries:\n")
	}
	// A date reads back into its string field as written, digits only.
	date := func(value string) string {
		if discDatePattern.MatchString(value) {
			return value
		}
		return yamlText(value)
	}
	for _, entry := range layout.Entries {
		fields := []string{fmt.Sprintf("lba: %d", entry.LBA)}
		switch {
		case entry.Dir != "":
			fields = append(fields, "dir: "+yamlText(entry.Dir), "date: "+date(entry.Date))
			if entry.Fixed != "" {
				fields = append(fields, "fixed: "+yamlText(entry.Fixed))
			}
		case entry.File != "":
			fields = append(fields, "file: "+yamlText(entry.File))
			if entry.Size != nil {
				fields = append(fields, fmt.Sprintf("size: %d", *entry.Size))
			}
			if entry.Type != "" {
				fields = append(fields, "type: "+yamlText(entry.Type))
			}
			fields = append(fields, "date: "+date(entry.Date))
		default:
			fields = append(fields, fmt.Sprintf("dummy: %d, form: %d", entry.Dummy, entry.Form))
		}
		fmt.Fprintf(&out, "  - {%s}\n", strings.Join(fields, ", "))
	}
	return out.Bytes()
}

// sectors returns how many sectors a file or dummy row takes, sizing a
// module's file by its module; mkpsxiso gives an empty file one sector.
func (entry *discEntry) sectors(modules map[string]*moduleSpec) int {
	if entry.Dummy > 0 {
		return entry.Dummy
	}
	size := 0
	if entry.Size != nil {
		size = *entry.Size
	} else if m := modules[entry.File]; m != nil {
		size = m.Size
	}
	unit := discDataSectorBytes
	if entry.Type == "mixed" {
		unit = discMixedSectorBytes
	}
	return max(1, (size+unit-1)/unit)
}

// discModuleFiles maps each module's disc path to its module.
func discModuleFiles(config *projectConfig) map[string]*moduleSpec {
	files := make(map[string]*moduleSpec, len(config.Modules))
	for _, m := range config.Modules {
		files[m.File] = m
	}
	return files
}

// validateDiscLayout checks the layout without the disc: each row's shape,
// that every file sits in a declared directory, that a file or dummy ends
// where the next row starts (only a directory record's length is mkpsxiso's
// to compute), and that each module's file has exactly one row.
func validateDiscLayout(layout *discLayout, config *projectConfig, fail func(string, ...any)) {
	label := discLayoutPath
	if layout.License == "" || strings.Contains(layout.License, "/") {
		fail("%s: license must name the extracted system-area file", label)
	}
	if layout.Volume.System == "" || layout.Volume.Volume == "" {
		fail("%s: volume needs a system and a volume identifier", label)
	}
	modules := discModuleFiles(config)
	dirs := map[string]bool{"": true}
	files := make(map[string]bool)
	for index, entry := range layout.Entries {
		row := fmt.Sprintf("%s: entry at lba %d", label, entry.LBA)
		kinds := 0
		for _, set := range []bool{entry.Dir != "", entry.File != "", entry.Dummy != 0} {
			if set {
				kinds++
			}
		}
		if kinds != 1 {
			fail("%s must be exactly one of dir, file or dummy", row)
			continue
		}
		if index == 0 && entry.LBA <= layout.Root.LBA || index > 0 && entry.LBA <= layout.Entries[index-1].LBA {
			fail("%s is out of sector order", row)
		}
		switch {
		case entry.Dir != "":
			if !discDatePattern.MatchString(entry.Date) || entry.Size != nil || entry.Type != "" || entry.Form != 0 {
				fail("%s: a directory takes only a date (YYYYMMDDHHMMSS) and fixed", row)
			}
			if path.Clean(entry.Dir) != entry.Dir || !dirs[parentDiscPath(entry.Dir)] || dirs[entry.Dir] {
				fail("%s: directory %q is not new in a declared directory", row, entry.Dir)
			}
			dirs[entry.Dir] = true
		case entry.File != "":
			if !discDatePattern.MatchString(entry.Date) || entry.Fixed != "" || entry.Form != 0 {
				fail("%s: a file takes a size, type and date (YYYYMMDDHHMMSS)", row)
			}
			if path.Clean(entry.File) != entry.File || !dirs[parentDiscPath(entry.File)] || files[entry.File] {
				fail("%s: file %q is not new in a declared directory", row, entry.File)
			}
			files[entry.File] = true
			if entry.Type != "" && entry.Type != "mixed" {
				fail("%s: type must be mixed or absent (data)", row)
			}
			_, module := modules[entry.File]
			switch {
			case module && entry.Size != nil:
				fail("%s: %s is a module's file; its module gives the size", row, entry.File)
			case !module && entry.Size == nil:
				fail("%s: %s needs a size", row, entry.File)
			case entry.Size != nil && (*entry.Size < 0 || entry.Type == "mixed" && *entry.Size%discMixedSectorBytes != 0):
				fail("%s: invalid size %d", row, *entry.Size)
			}
		default:
			if entry.Dummy < 0 || entry.Form != 1 && entry.Form != 2 || entry.Date != "" || entry.Size != nil || entry.Type != "" || entry.Fixed != "" {
				fail("%s: a dummy takes a sector count and form 1 or 2", row)
			}
		}
		if entry.Dir == "" && index+1 < len(layout.Entries) {
			if end, next := entry.LBA+entry.sectors(modules), layout.Entries[index+1].LBA; end != next {
				fail("%s ends at sector %d, but the next entry starts at %d", row, end, next)
			}
		}
	}
	for _, m := range config.Modules {
		if !files[m.File] {
			fail("%s: module %s's file %s has no entry", label, m.ID, m.File)
		}
	}
}

func parentDiscPath(name string) string {
	if index := strings.LastIndexByte(name, '/'); index >= 0 {
		return name[:index]
	}
	return ""
}

// mkpsxisoProject writes the mkpsxiso project of the layout. A file's source
// is its rebuilt module in built (module id to path) or its extracted copy
// under filesRoot. A dummy goes in the directory of the row before it, as
// dumpsxiso writes it.
func (layout *discLayout) mkpsxisoProject(config *projectConfig, filesRoot string, built map[string]string) []byte {
	type node struct {
		entry    *discEntry
		children []*node
	}
	root := &node{}
	dirs := map[string]*node{"": root}
	current := root
	for index := range layout.Entries {
		entry := &layout.Entries[index]
		item := &node{entry: entry}
		switch {
		case entry.Dir != "":
			dirs[parentDiscPath(entry.Dir)].children = append(dirs[parentDiscPath(entry.Dir)].children, item)
			dirs[entry.Dir] = item
			current = item
		case entry.File != "":
			current = dirs[parentDiscPath(entry.File)]
			current.children = append(current.children, item)
		default:
			current.children = append(current.children, item)
		}
	}
	modules := discModuleFiles(config)
	source := func(name string) string {
		if m := modules[name]; m != nil && built[m.ID] != "" {
			return xmlAttribute(built[m.ID])
		}
		return xmlAttribute(filesRoot + "/" + name)
	}
	var out bytes.Buffer
	v, a, r := layout.Volume, layout.Attributes, layout.Root
	out.WriteString("<iso_project image_name=\"mkpsxiso.bin\" cue_sheet=\"mkpsxiso.cue\">\n    <track type=\"data\" cdvd_style=\"false\">\n")
	fmt.Fprintf(&out, "        <identifiers system=\"%s\" application=\"%s\" volume=\"%s\" volume_set=\"%s\" publisher=\"%s\" data_preparer=\"%s\" creation_date=\"%s\" modification_date=\"%s\"/>\n",
		xmlAttribute(v.System), xmlAttribute(v.Application), xmlAttribute(v.Volume), xmlAttribute(v.VolumeSet), xmlAttribute(v.Publisher),
		xmlAttribute(v.DataPreparer), xmlAttribute(v.CreationDate), xmlAttribute(v.ModificationDate))
	fmt.Fprintf(&out, "        <license file=\"%s\"/>\n", source(layout.License))
	fmt.Fprintf(&out, "        <default_attributes gmt_offs=\"%d\" xa_attrib=\"%d\" xa_perm=\"%d\" xa_gid=\"%d\" xa_uid=\"%d\" hidden=\"%d\"/>\n",
		a.GMTOffset, a.XAAttrib, a.XAPerm, a.XAGID, a.XAUID, a.Hidden)
	fmt.Fprintf(&out, "        <directory_tree xa_perm=\"%d\" xa_gid=\"%d\" xa_uid=\"%d\" hidden=\"%d\" offs=\"%d\">\n", r.XAPerm, r.XAGID, r.XAUID, r.Hidden, r.LBA)
	var write func(children []*node, depth int)
	write = func(children []*node, depth int) {
		indent := strings.Repeat("    ", depth)
		for _, child := range children {
			entry := child.entry
			switch {
			case entry.Dir != "":
				fmt.Fprintf(&out, "%s<dir name=\"%s\" source=\"%s\" date=\"%s\" xa_perm=\"%d\" xa_gid=\"%d\" xa_uid=\"%d\" offs=\"%d\">\n",
					indent, xmlAttribute(path.Base(entry.Dir)), source(entry.Dir), entry.Date, r.XAPerm, r.XAGID, r.XAUID, entry.LBA)
				write(child.children, depth+1)
				fmt.Fprintf(&out, "%s</dir>\n", indent)
			case entry.File != "":
				kind := "data"
				if entry.Type != "" {
					kind = entry.Type
				}
				fmt.Fprintf(&out, "%s<file name=\"%s\" source=\"%s\" type=\"%s\" date=\"%s\" offs=\"%d\"/>\n",
					indent, xmlAttribute(path.Base(entry.File)), source(entry.File), kind, entry.Date, entry.LBA)
			default:
				submode := 0
				if entry.Form == 2 {
					submode = 0x20
				}
				fmt.Fprintf(&out, "%s<dummy sectors=\"%d\" type=\"%d\" offs=\"%d\"/>\n", indent, entry.Dummy, submode, entry.LBA)
			}
		}
	}
	write(root.children, 3)
	out.WriteString("        </directory_tree>\n    </track>\n</iso_project>\n")
	return out.Bytes()
}
