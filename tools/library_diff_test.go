package main

import (
	"strings"
	"testing"
)

func TestLibraryCandidateOwnership(t *testing.T) {
	m := &moduleSpec{
		ID: "main", Load: 0x80010000, Size: 0x1000, Profile: defaultProfile,
		Libraries: []librarySpec{{ID: "libgpu", Addr: 0x80010200, End: 0x80010300}},
	}
	for _, test := range []struct {
		name    string
		addr    uint64
		size    int
		source  string
		profile string
		want    string
	}{
		{"valid", 0x80010200, 20, "build/research/f.c", "", ""},
		{"last-word", 0x800102fc, 4, "build/research/f.c", "", ""},
		{"crosses-library", 0x800102fc, 8, "build/research/f.c", "", "contained"},
		{"game-function", 0x80010100, 8, "build/research/f.c", "", "contained"},
		{"unaligned", 0x80010202, 8, "build/research/f.c", "", "invalid"},
		{"zero-size", 0x80010200, 0, "build/research/f.c", "", "invalid"},
		{"beyond-32-bit", 0x180010200, 4, "build/research/f.c", "", "invalid"},
		{"wrong-filename", 0x80010200, 8, "build/research/other.c", "", "named"},
		{"unknown-profile", 0x80010200, 8, "build/research/f.c", "fake", "unknown compiler"},
	} {
		t.Run(test.name, func(t *testing.T) {
			f, err := libraryCandidate(m, "f", test.source, test.profile, test.addr, test.size)
			if test.want == "" {
				if err != nil || f == nil || f.Profile != defaultProfile {
					t.Fatalf("candidate = %#v, error = %v", f, err)
				}
			} else if err == nil || !strings.Contains(err.Error(), test.want) {
				t.Fatalf("error = %v, want %q", err, test.want)
			}
		})
	}
}

func TestLibraryCandidateRodata(t *testing.T) {
	m := &moduleSpec{ID: "main", Load: 0x80010000, Size: 0x1000}
	for _, test := range []struct {
		name string
		addr uint64
		size int
		want string
	}{
		{"before-library", 0x800100cc, 60, ""},
		{"last-word", 0x80010ffc, 4, ""},
		{"outside-module", 0x80010ffc, 8, "invalid"},
		{"overlap", 0x80010210, 8, "overlaps"},
		{"spans-text", 0x800101fc, 40, "overlaps"},
		{"unaligned", 0x800100ce, 8, "invalid"},
		{"negative-size", 0x800100cc, -4, "invalid"},
		{"beyond-32-bit", 0x1800100cc, 4, "invalid"},
	} {
		t.Run(test.name, func(t *testing.T) {
			f := &functionSpec{Addr: 0x80010200, Size: 32}
			err := libraryCandidateRodata(m, f, test.addr, test.size)
			if test.want == "" {
				if err != nil || len(f.Rodata) != 1 || f.Rodata[0].Size != test.size {
					t.Fatalf("rodata = %#v, error = %v", f.Rodata, err)
				}
			} else if err == nil || !strings.Contains(err.Error(), test.want) || len(f.Rodata) != 0 {
				t.Fatalf("error = %v, want %q; rodata = %#v", err, test.want, f.Rodata)
			}
		})
	}
}
