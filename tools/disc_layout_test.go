package main

import (
	"os"
	"path/filepath"
	"strings"
	"testing"
)

const testDiscLayoutYAML = `# The test disc.
` + testDiscHeaderYAML + `
entries:
  - {lba: 23, file: SYSTEM.CNF, size: 68, date: 19970912162300}
  - {lba: 24, file: SCUS_942.21, date: 19971017183704}
  - {lba: 26, file: EMPTY.BIN, size: 0, date: 19971017183704}
  - {lba: 27, dummy: 3, form: 2}
  - {lba: 30, dir: MAP, date: 19971017183721, fixed: "a GNS row stores each file's sector"}
  - {lba: 31, file: MAP/MAP001.GNS, size: 2049, date: 19971017183721}
  - {lba: 33, dir: OPEN, date: 19971017183721}
  - {lba: 34, dummy: 2, form: 2}
  - {lba: 36, file: OPEN/FFTST.STR, size: 4672, type: mixed, date: 19971014194710}
  - {lba: 38, dummy: 10, form: 1}
`

func discTestProject(t *testing.T, disc string) project {
	return testProject(t, map[string]string{
		"target/main.yaml":       testMainYAML,
		discLayoutPath:           disc,
		"src/main/main_first.c":  "void main_first(void) {}\n",
		"src/main/main_second.c": "void main_second(void) {}\n",
	})
}

func TestDiscLayoutRoundTripsAndPlacesModules(t *testing.T) {
	p := discTestProject(t, testDiscLayoutYAML)
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	if err := p.validateProjectConfig(config); err != nil {
		t.Fatal(err)
	}
	if got := string(renderDiscLayout(config.Disc)); got != testDiscLayoutYAML {
		t.Fatalf("not canonical:\n%s", got)
	}
	if main, _ := config.module("main"); main.LBA != 24 {
		t.Fatalf("main at sector %d", main.LBA)
	}
}

func TestDiscLayoutRejectsInvalidEntries(t *testing.T) {
	for name, edit := range map[string][2]string{
		"gap after a dummy":     {"dummy: 3,", "dummy: 2,"},
		"file overlaps":         {"size: 68,", "size: 2049,"},
		"module size":           {"file: SCUS_942.21,", "file: SCUS_942.21, size: 4096,"},
		"data file needs size":  {"file: SYSTEM.CNF, size: 68,", "file: SYSTEM.CNF,"},
		"undeclared directory":  {"file: OPEN/FFTST.STR", "file: MOVIE/FFTST.STR"},
		"mixed sector size":     {"size: 4672,", "size: 4673,"},
		"dummy form":            {"form: 1}", "form: 3}"},
		"before the root":       {"{lba: 23,", "{lba: 21,"},
		"two kinds in one row":  {"dummy: 3, form: 2}", "dummy: 3, form: 2, file: X.BIN}"},
		"fixed file":            {"size: 0,", "size: 0, fixed: no,"},
		"missing module's file": {"  - {lba: 24, file: SCUS_942.21, date: 19971017183704}\n", "  - {lba: 24, dummy: 2, form: 2}\n"},
	} {
		t.Run(name, func(t *testing.T) {
			if !strings.Contains(testDiscLayoutYAML, edit[0]) {
				t.Fatalf("edit %q does not apply", edit[0])
			}
			p := discTestProject(t, strings.Replace(testDiscLayoutYAML, edit[0], edit[1], 1))
			config, err := p.loadProjectConfig()
			if err == nil {
				err = p.validateProjectConfig(config)
			}
			if err == nil || !strings.Contains(err.Error(), discLayoutPath) {
				t.Fatalf("invalid layout accepted: %v", err)
			}
		})
	}
	p := discTestProject(t, testDiscLayoutYAML)
	if err := os.Remove(filepath.Join(p.root, filepath.FromSlash(discLayoutPath))); err != nil {
		t.Fatal(err)
	}
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	if err := p.validateProjectConfig(config); err == nil || !strings.Contains(err.Error(), "disc.yaml is missing") {
		t.Fatalf("missing layout: %v", err)
	}
}

func TestDiscLayoutWritesTheMkpsxisoProject(t *testing.T) {
	p := discTestProject(t, testDiscLayoutYAML)
	config, err := p.loadProjectConfig()
	if err != nil {
		t.Fatal(err)
	}
	got := string(config.Disc.mkpsxisoProject(config, "/files", map[string]string{"main": "/out/SCUS_942.21"}))
	want := `<iso_project image_name="mkpsxiso.bin" cue_sheet="mkpsxiso.cue">
    <track type="data" cdvd_style="false">
        <identifiers system="PLAYSTATION" application="PLAYSTATION" volume="TEST" volume_set="TEST" publisher="TEST" data_preparer="TEST" creation_date="1997101718372100+36" modification_date="1997101718372100+36"/>
        <license file="/files/license_data.dat"/>
        <default_attributes gmt_offs="36" xa_attrib="56" xa_perm="1" xa_gid="10752" xa_uid="10752" hidden="1"/>
        <directory_tree xa_perm="1365" xa_gid="0" xa_uid="0" hidden="0" offs="22">
            <file name="SYSTEM.CNF" source="/files/SYSTEM.CNF" type="data" date="19970912162300" offs="23"/>
            <file name="SCUS_942.21" source="/out/SCUS_942.21" type="data" date="19971017183704" offs="24"/>
            <file name="EMPTY.BIN" source="/files/EMPTY.BIN" type="data" date="19971017183704" offs="26"/>
            <dummy sectors="3" type="32" offs="27"/>
            <dir name="MAP" source="/files/MAP" date="19971017183721" xa_perm="1365" xa_gid="0" xa_uid="0" offs="30">
                <file name="MAP001.GNS" source="/files/MAP/MAP001.GNS" type="data" date="19971017183721" offs="31"/>
            </dir>
            <dir name="OPEN" source="/files/OPEN" date="19971017183721" xa_perm="1365" xa_gid="0" xa_uid="0" offs="33">
                <dummy sectors="2" type="32" offs="34"/>
                <file name="FFTST.STR" source="/files/OPEN/FFTST.STR" type="mixed" date="19971014194710" offs="36"/>
                <dummy sectors="10" type="0" offs="38"/>
            </dir>
        </directory_tree>
    </track>
</iso_project>
`
	if got != want {
		t.Fatalf("project:\n%s", got)
	}
}
