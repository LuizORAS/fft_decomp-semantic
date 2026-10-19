package main

import (
	"encoding/json"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

const auditLocalLeft = `int left(struct item* track, int component) {
    int start_value;
    int time[4];
    start_value = track->value;
    time[component] = left_callee(start_value);
    return time[component] + track->start_value;
}`

const auditLocalRight = `int right(struct item* record, int index) {
    int key_value;
    int ticks[4];
    key_value = record->value;
    ticks[index] = right_callee(key_value);
    return ticks[index] + record->start_value;
}`

func TestShareAuditLocalNamesAreOptInAndRetainMembers(t *testing.T) {
	p, config := shareAuditFixture(t, auditLocalLeft, auditLocalRight, shareAuditHeader)
	before, err := p.shareAudit(config, shareAuditOptions{})
	if err != nil || len(before.Groups) != 0 {
		t.Fatalf("default changed: %+v %v", before, err)
	}
	after, err := p.shareAudit(config, shareAuditOptions{normalizeLocals: true})
	if err != nil || len(after.Groups) != 1 {
		t.Fatalf("alpha-equivalent fixture: %+v %v", after, err)
	}
	for _, entry := range after.Groups[0].Participants {
		if entry.LocalNormalization.Status != "normalized" || len(entry.LocalNormalization.Roles) != 4 {
			t.Fatalf("missing correspondence: %+v", entry)
		}
		if entry.LocalNormalization.Roles[0].Kind != "parameter" || entry.LocalNormalization.Roles[2].Kind != "local" ||
			!strings.Contains(entry.LocalNormalization.Roles[3].Declaration, "[4]") {
			t.Fatalf("lost declaration: %+v", entry.LocalNormalization)
		}
	}
	// A member with the same spelling as a local must remain a member token.
	p, config = shareAuditFixture(t, auditLocalLeft,
		strings.Replace(auditLocalRight, "record->start_value", "record->key_value", 1), shareAuditHeader)
	report, err := p.shareAudit(config, shareAuditOptions{normalizeLocals: true})
	if err != nil || len(report.Groups) != 0 {
		t.Fatalf("member difference hidden: %+v %v", report, err)
	}
}

func TestShareAuditLocalModeRetainsCompleteTypeAndExpressionTokens(t *testing.T) {
	cases := map[string]string{
		"local signedness":       strings.Replace(auditLocalRight, "int key_value", "unsigned int key_value", 1),
		"pointer qualifier":      strings.Replace(auditLocalRight, "struct item* record", "struct item* const record", 1),
		"return qualifier":       `const struct item* right(struct item* record) { return record; }`,
		"parameter qualifier":    strings.Replace(auditLocalRight, "struct item* record", "const struct item* record", 1),
		"array extent":           strings.Replace(auditLocalRight, "ticks[4]", "ticks[5]", 1),
		"cast":                   strings.Replace(auditLocalRight, "right_callee(key_value)", "right_callee((short)key_value)", 1),
		"operator":               strings.Replace(auditLocalRight, " + record", " - record", 1),
		"literal":                strings.Replace(auditLocalRight, "record->value;", "record->value + 1;", 1),
		"unknown identifier":     strings.Replace(auditLocalRight, "record->value;", "record->value + UNKNOWN_VALUE;", 1),
		"type spelling":          strings.Replace(auditLocalRight, "int key_value", "signed int key_value", 1),
		"parameter array extent": `int right(struct item* record, int index[5]) { return record->value; }`,
	}
	for name, right := range cases {
		t.Run(name, func(t *testing.T) {
			left := auditLocalLeft
			if name == "return qualifier" {
				left = `struct item* left(struct item* track) { return track; }`
			}
			if name == "parameter array extent" {
				left = `int left(struct item* track, int component[4]) { return track->value; }`
			}
			p, config := shareAuditFixture(t, left, right, shareAuditHeader)
			report, err := p.shareAudit(config, shareAuditOptions{normalizeLocals: true})
			if err != nil || len(report.Groups) != 0 {
				t.Fatalf("difference hidden: %+v %v", report, err)
			}
		})
	}
}

func TestShareAuditLocalModeRetainsHeaderTagNamespace(t *testing.T) {
	cases := []struct{ name, left, right, keyword, tag string }{
		{"struct parameter", `int left(struct item* item) { return item->value; }`,
			`int right(struct item* record) { return record->value; }`, "struct", "item"},
		{"union parameter", `int left(union item* item) { return item->value; }`,
			`int right(union item* record) { return record->value; }`, "union", "item"},
		{"enum parameter", `int left(enum item item) { return item; }`,
			`int right(enum item kind) { return kind; }`, "enum", "item"},
		{"return tag and local", `struct item* left(int n) { int item; item = n; return left_callee(item); }`,
			`struct item* right(int count) { int result; result = count; return right_callee(result); }`, "struct", "item"},
		{"return tag and self", `struct left* left(int n) { return left_callee(n); }`,
			`struct left* right(int count) { return right_callee(count); }`, "struct", "left"},
	}
	for _, tc := range cases {
		t.Run(tc.name, func(t *testing.T) {
			p, config := shareAuditFixture(t, tc.left, tc.right, shareAuditHeader)
			report, err := p.shareAudit(config, shareAuditOptions{normalizeLocals: true})
			if err != nil || len(report.Groups) != 1 {
				t.Fatalf("same tag and renamed object did not group: %+v %v", report, err)
			}
			scanner, err := newShareAuditScanner(p, config)
			if err != nil {
				t.Fatal(err)
			}
			for name, source := range map[string]string{"left": tc.left, "right": tc.right} {
				tokens, err := tokenizeC(source)
				if err != nil {
					t.Fatal(err)
				}
				parameters, body, found := shareAuditDefinition(tokens, name)
				if !found {
					t.Fatal("missing fixture definition")
				}
				_, header, status := scanner.normalizeLocalRoles(source, tokens, parameters, body,
					map[string]uint32{"left_callee": 1, "right_callee": 2}, name)
				if status.Status != "normalized" || !strings.Contains(strings.Join(header, " "), tc.keyword+" "+tc.tag) {
					t.Fatalf("tag was rewritten: %v %+v", header, status)
				}
			}
		})
	}
}

func TestShareAuditLocalModeNeverMergesDifferentHeaderTags(t *testing.T) {
	for _, keyword := range []string{"struct", "union", "enum"} {
		t.Run(keyword+" parameter", func(t *testing.T) {
			pointer := "*"
			value := "->value"
			if keyword == "enum" {
				pointer, value = "", ""
			}
			left := "int left(" + keyword + " item" + pointer + " item) { return item" + value + "; }"
			right := "int right(" + keyword + " other" + pointer + " other) { return other" + value + "; }"
			p, config := shareAuditFixture(t, left, right, shareAuditHeader)
			report, err := p.shareAudit(config, shareAuditOptions{normalizeLocals: true})
			if err != nil || len(report.Groups) != 0 {
				t.Fatalf("different tags merged: %+v %v", report, err)
			}
		})
	}
	p, config := shareAuditFixture(t,
		`struct item* left(int n) { int item; item = n; return left_callee(item); }`,
		`struct other* right(int count) { int other; other = count; return right_callee(other); }`, shareAuditHeader)
	report, err := p.shareAudit(config, shareAuditOptions{normalizeLocals: true})
	if err != nil || len(report.Groups) != 0 {
		t.Fatalf("different return tags merged: %+v %v", report, err)
	}
}

func TestShareAuditLocalModeFallsBackForBoundedGrammar(t *testing.T) {
	cases := map[string]string{
		"nested shadow":             `int left(int n) { int i; { int i; i = n; } return i; }`,
		"nested unique local":       `int left(int n) { if (n) { int i; return i; } return n; }`,
		"linker symbol shadow":      `int left(int n) { int left_callee; return left_callee + n; }`,
		"use before declaration":    `int left(int n) { value = n; int value; return value; }`,
		"static local":              `int left(int n) { static int i; return i + n; }`,
		"extern local":              `int left(int n) { extern int i; return i + n; }`,
		"local typedef":             `int left(int n) { typedef int local_t; local_t i; return i; }`,
		"local tag definition":      `int left(int n) { struct local { int x; } value; return value.x; }`,
		"function pointer":          `int left(int (*callback)(int)) { return callback(1); }`,
		"function declaration":      `int left(int n) { int callback(int); return callback(n); }`,
		"for declaration":           `int left(int n) { for (int i = 0; i < n; i++) {} return n; }`,
		"aggregate initializer":     `int left(int n) { int values[2] = {1, 2}; return values[n]; }`,
		"complex extent":            `int left(int n) { int values[sizeof(int)]; return values[n]; }`,
		"body preprocessing":        "int left(int n) {\n#define VALUE n\nreturn VALUE; }",
		"unknown macro":             `int left(int n) { return UNKNOWN(n); }`,
		"unknown type":              `int left(int n) { private_t value; return value; }`,
		"ambiguous expression":      `int left(int n) { int value; n * value; return value; }`,
		"unprototyped list":         `int left() { return 0; }`,
		"tag namespace collision":   `int left(int n) { int node; return sizeof(struct node) + node; }`,
		"label namespace collision": `int left(int n) { int label; label: return label; }`,
		"mixed complex declarator":  `int left(int n) { int value, (*callback)(int); return value; }`,
	}
	for name, source := range cases {
		t.Run(name, func(t *testing.T) {
			p, config := shareAuditFixture(t, source, shareAuditRight, shareAuditHeader)
			scanner, err := newShareAuditScanner(p, config)
			if err != nil {
				t.Fatal(err)
			}
			scanner.normalizeLocals = true
			main, _ := config.module("main")
			entry, _, _, reason, err := scanner.entry(main, &main.Functions[0])
			if err != nil || reason != "" || entry.LocalNormalization == nil || entry.LocalNormalization.Status != "exact_fallback" {
				t.Fatalf("unsupported syntax normalized: %+v %s %v", entry, reason, err)
			}
		})
	}
}

func TestShareAuditLocalModeIsDeterministicAndReadOnly(t *testing.T) {
	p, config := shareAuditFixture(t, auditLocalLeft, auditLocalRight, shareAuditHeader)
	before := map[string]string{}
	for _, path := range []string{"src/a/left.c", "src/b/right.c", "include/contracts.h"} {
		data, err := os.ReadFile(filepath.Join(p.root, path))
		if err != nil {
			t.Fatal(err)
		}
		before[path] = string(data)
	}
	a, err := p.shareAudit(config, shareAuditOptions{normalizeLocals: true})
	if err != nil {
		t.Fatal(err)
	}
	config.Modules[0], config.Modules[1] = config.Modules[1], config.Modules[0]
	b, err := p.shareAudit(config, shareAuditOptions{normalizeLocals: true})
	if err != nil {
		t.Fatal(err)
	}
	ja, _ := json.Marshal(a)
	jb, _ := json.Marshal(b)
	if string(ja) != string(jb) {
		t.Fatal("iteration order changed report")
	}
	for path, want := range before {
		got, err := os.ReadFile(filepath.Join(p.root, path))
		if err != nil || string(got) != want {
			t.Fatalf("audit changed %s: %v", path, err)
		}
	}
}
