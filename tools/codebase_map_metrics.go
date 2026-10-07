// codebase_map_metrics.go measures the build-2 "done" criteria for the map:
// summary coverage, names that still carry an address or a bare numeric
// suffix, weak parameter and local names, and header blocks that mix
// subsystems or lack a purpose comment. It also writes functions.tsv, the
// Bases views and, once, a starting .obsidian configuration.
package main

import (
	"fmt"
	"regexp"
	"sort"
	"strings"
)

const mapMetricsPage = "Metrics"

var (
	mapAddressNamePattern   = regexp.MustCompile(`[0-9a-f]{8}`)
	mapNumericSuffixPattern = regexp.MustCompile(`^(.*)_[0-9]{1,2}$`)
	// mapWeakNamePattern flags parameter and local names that say nothing:
	// MIPS register names, unknown/var/arg/local/field/val/tmp placeholders,
	// numbered temps and single letters other than the usual i j k n x y z w
	// h u v r g b. Texture coordinates such as u0/v1 and a bare `temp` (a swap)
	// are accepted.
	mapWeakNamePattern = regexp.MustCompile(`^(?:[atsr][0-9]+|sp|ra|at|fp|(?:unk|unknown)[A-Za-z0-9_]*|(?:var|arg|local|field|val|tmp)_?[0-9]*|temp_?[0-9]+|[acdeflmopqst])$`)
	// mapGenericNamePattern flags names that are acceptable in a short
	// function but often hide a better one; they are reviewed case by case.
	mapGenericNamePattern = regexp.MustCompile(`^(?:value|data|ptr|temp|buf|buffer)[0-9]*$`)
)

// declaredNames returns the parameter names of a definition and the names
// declared at the start of its blocks.
func (index *codebaseIndex) declaredNames(f *mapFunction, tokens []cToken) []string {
	var names []string
	open := -1
	for position := 0; position+1 < len(tokens); position++ {
		if tokens[position].text == f.name && tokens[position+1].text == "(" {
			open = position + 1
			break
		}
	}
	if open < 0 {
		return nil
	}
	close := matchingToken(tokens, open)
	if close < 0 {
		return nil
	}
	for _, part := range splitTopLevel(tokens[open+1:close], ",") {
		if name := declaredName(part, true); name != "" {
			names = append(names, name)
		}
	}
	body := close + 1
	for body < len(tokens) && tokens[body].text != "{" {
		body++
	}
	depth := 0
	statementStart := true
	for position := body; position < len(tokens); position++ {
		token := tokens[position]
		switch token.text {
		case "(", "[":
			depth++
			statementStart = false
			continue
		case ")", "]":
			depth--
			statementStart = false
			continue
		case "{", "}", ";":
			statementStart = depth == 0
			continue
		}
		if !statementStart {
			continue
		}
		statementStart = false
		if !index.startsDeclaration(tokens, position) {
			continue
		}
		end := position
		for nested := 0; end < len(tokens); end++ {
			switch tokens[end].text {
			case "(", "[", "{":
				nested++
			case ")", "]", "}":
				nested--
			}
			if nested == 0 && tokens[end].text == ";" {
				break
			}
		}
		for n, part := range splitTopLevel(tokens[position:end], ",") {
			if name := declaredName(part, n == 0); name != "" {
				names = append(names, name)
			}
		}
		position = end - 1
	}
	return names
}

// startsDeclaration reports whether a block statement begins with a type.
func (index *codebaseIndex) startsDeclaration(tokens []cToken, position int) bool {
	token := tokens[position]
	if token.kind != cIdent || position+1 >= len(tokens) {
		return false
	}
	switch next := tokens[position+1].text; {
	case next == "(" || next == "=" || next == ":" || next == "." || next == "->" || next == "[":
		return false
	}
	text := token.text
	if declarationBaseWords[text] || declarationQualifiers[text] || text == "static" || isTagKeyword(text) || index.types[text] != nil {
		return true
	}
	// A type the headers do not declare (a typedef in the source itself):
	// `T name` or `T* name` cannot be an expression statement.
	next := tokens[position+1]
	if next.kind == cIdent {
		return true
	}
	return next.text == "*" && position+2 < len(tokens) && (tokens[position+2].kind == cIdent || tokens[position+2].text == "*")
}

// declaredName returns the name one declarator binds; withSpecifier is true
// when the part still starts with the type.
func declaredName(part []cToken, withSpecifier bool) string {
	declarator := part
	if withSpecifier {
		if len(part) == 1 && (part[0].text == "void" || part[0].text == "...") {
			return ""
		}
		_, begin, _ := parseDeclarationSpecifier(part)
		declarator = part[begin:]
	}
	if equals := topLevelIndex(declarator, "="); equals >= 0 {
		declarator = declarator[:equals]
	}
	if n := len(declarator); n >= 4 && declarator[n-1].text == ")" && declarator[n-2].kind == cString {
		declarator = declarator[:n-4]
	}
	if bound := boundIdentifier(declarator); bound >= 0 {
		return declarator[bound].text
	}
	return ""
}

func mapAddressName(name string) bool { return mapAddressNamePattern.MatchString(name) }

// mapNumericSuffix returns the base of a name ending in _N or _NN.
func mapNumericSuffix(name string) (string, bool) {
	match := mapNumericSuffixPattern.FindStringSubmatch(name)
	if match == nil {
		return "", false
	}
	return match[1], true
}

// suffixJustified reports whether a numbered name's summary names its
// sibling family, which is the accepted reason to keep the number.
func suffixJustified(f *mapFunction) bool {
	base, numbered := mapNumericSuffix(f.name)
	return numbered && strings.Contains(f.summary, base)
}

func (f *mapFunction) weakNames() []string    { return f.namesMatching(mapWeakNamePattern) }
func (f *mapFunction) genericNames() []string { return f.namesMatching(mapGenericNamePattern) }

func (f *mapFunction) namesMatching(pattern *regexp.Regexp) []string {
	var names []string
	for _, name := range f.locals {
		if pattern.MatchString(name) {
			names = appendUnique(names, name)
		}
	}
	return names
}

// mapFamily groups functions by source directory for the coverage tables.
func mapFamily(f *mapFunction) string {
	parts := strings.Split(f.source, "/")
	if len(parts) > 2 && parts[1] == "psyq" {
		return "psyq"
	}
	if len(parts) > 1 {
		return parts[1]
	}
	return f.source
}

type mapLabelUse struct {
	header, label string
	purpose       bool
	subsystems    []string
}

// labelUses lists every label block in include/fft/ with the code subsystems
// of the prototypes under it.
func (index *codebaseIndex) labelUses() []mapLabelUse {
	var uses []mapLabelUse
	for _, path := range sortedKeys(index.headers) {
		if !strings.HasPrefix(path, "include/fft/") {
			continue
		}
		header := index.headers[path]
		for _, label := range header.labels {
			use := mapLabelUse{header: path, label: label.name, purpose: label.purpose}
			for _, name := range header.prototypes {
				if index.prototypes[name].label != label.name {
					continue
				}
				subsystem := mapSubsystem(name, "src/"+strings.Split(strings.TrimPrefix(path, "include/fft/"), ".")[0]+"/")
				if f := index.functions[name]; f != nil {
					subsystem = f.subsystem
				}
				use.subsystems = appendUnique(use.subsystems, subsystem)
			}
			sort.Strings(use.subsystems)
			uses = append(uses, use)
		}
	}
	return uses
}

func percent(part, whole int) string {
	if whole == 0 {
		return "-"
	}
	return fmt.Sprintf("%.1f%%", 100*float64(part)/float64(whole))
}

func (vault *mapVault) metricsPage() string {
	index := vault.index
	type family struct{ functions, prose, any, weakFunctions, weakNames, genericNames int }
	families := map[string]*family{}
	var addressNamed, unjustified []*mapFunction
	numbered, prose, weakNames, weakFunctions, genericNames := 0, 0, 0, 0, 0
	for _, name := range sortedKeys(index.functions) {
		f := index.functions[name]
		group := families[mapFamily(f)]
		if group == nil {
			group = &family{}
			families[mapFamily(f)] = group
		}
		group.functions++
		if proseSummary(f.summary) {
			group.prose++
			prose++
		}
		if f.summary != "" {
			group.any++
		}
		if weak := f.weakNames(); len(weak) > 0 {
			group.weakFunctions++
			group.weakNames += len(weak)
			weakFunctions++
			weakNames += len(weak)
		}
		group.genericNames += len(f.genericNames())
		genericNames += len(f.genericNames())
		if mapAddressName(name) {
			addressNamed = append(addressNamed, f)
		}
		if _, ok := mapNumericSuffix(name); ok {
			numbered++
			if !suffixJustified(f) {
				unjustified = append(unjustified, f)
			}
		}
	}
	uses := index.labelUses()
	purposed, mixed := 0, []mapLabelUse{}
	for _, use := range uses {
		if use.purpose {
			purposed++
		}
		if len(use.subsystems) > 1 {
			mixed = append(mixed, use)
		}
	}
	linked, quirks := 0, 0
	var unlinked []*mapQuirk
	for _, section := range index.quirks {
		for _, quirk := range section.entries {
			quirks++
			if len(quirk.keys) > 0 {
				linked++
			} else {
				unlinked = append(unlinked, quirk)
			}
		}
	}
	unnamedDebts := 0
	for _, debt := range index.debts {
		if !index.quirksCovers(debt.comment) {
			unnamedDebts++
		}
	}

	var b strings.Builder
	total := len(index.functions)
	b.WriteString(frontmatter(
		mapProperty{"type", "report"},
		mapProperty{"functions", total},
		mapProperty{"prose_summaries", prose},
		mapProperty{"address_names", len(addressNamed)},
		mapProperty{"numeric_suffixes", numbered},
		mapProperty{"unjustified_suffixes", len(unjustified)},
		mapProperty{"weak_names", weakNames},
		mapProperty{"generic_names", genericNames},
		mapProperty{"labels", len(uses)},
		mapProperty{"labels_with_purpose", purposed},
		mapProperty{"mixed_labels", len(mixed)},
		mapProperty{"stale_mentions", len(index.stale)},
		mapProperty{"mechanic_pages", vault.docCount("mechanic")},
		mapProperty{"guides", vault.docCount("guide")},
		mapProperty{"functions_in_scope", len(vault.mechanic)},
	))
	fmt.Fprintf(&b, "# %s\n\n", mapMetricsPage)
	b.WriteString("The measurable build-2 \"done\" criteria, computed by `make map` from the code. Function counts are by name; a function linked into several overlays counts once.\n\n")
	b.WriteString("| Criterion | Now | Goal |\n|---|---|---|\n")
	b.WriteString(tableRow("Functions with a prose summary", fmt.Sprintf("%d / %d (%s)", prose, total, percent(prose, total)), "100%"))
	b.WriteString(tableRow("Function names with an address", fmt.Sprint(len(addressNamed)), "only empty functions with no reference, each commented"))
	b.WriteString(tableRow("Numbered names whose summary does not name the sibling", fmt.Sprintf("%d of %d", len(unjustified), numbered), "0"))
	b.WriteString(tableRow("Weak parameter and local names", fmt.Sprintf("%d in %d functions", weakNames, weakFunctions), "0 in tiers 1-2"))
	b.WriteString(tableRow("Generic parameter and local names", fmt.Sprint(genericNames), "reviewed case by case in tiers 1-2"))
	b.WriteString(tableRow("Header blocks (include/fft) with a purpose comment", fmt.Sprintf("%d / %d", purposed, len(uses)), "100%"))
	b.WriteString(tableRow("Header blocks whose prototypes mix subsystems", fmt.Sprint(len(mixed)), "0, or a documented exception"))
	b.WriteString(tableRow("Stale mentions", fmt.Sprintf("[[%s]]: %d", mapStalePage, len(index.stale)), "0"))
	b.WriteString(tableRow("Mechanic pages (docs/mechanics)", fmt.Sprint(vault.docCount("mechanic")), "tier 1: 8 plus Engine core, tier 2: 4, tier 3: 9, tier 4: 4"))
	b.WriteString(tableRow("Guides (docs/guides)", fmt.Sprint(vault.docCount("guide")), "at least 8"))
	b.WriteString(tableRow("Functions in a mechanic page scope", fmt.Sprintf("%d / %d (%s)", len(vault.mechanic), total, percent(len(vault.mechanic), total)), "every tier 1-2 function"))
	b.WriteString(tableRow("QUIRKS.md entries linked to a page", fmt.Sprintf("%d / %d", linked, quirks), "every entry that names a symbol (general entries listed below)"))
	b.WriteString(tableRow("Debt comments whose subject QUIRKS.md does not name", fmt.Sprint(unnamedDebts), "0"))

	kinds := map[string]int{}
	wrapped := 0
	for _, region := range index.regions {
		kinds[region.kind]++
		if region.function != "" {
			wrapped++
		}
	}
	var kindCounts []string
	for _, kind := range sortedKeys(kinds) {
		kindCounts = append(kindCounts, fmt.Sprintf("%d %s", kinds[kind], kind))
	}
	b.WriteString(tableRow("Code kept as original bytes (target/ regions)", strings.Join(kindCounts, ", ")+fmt.Sprintf("; %d wrapped by a C file", wrapped), "documented on each module page"))
	b.WriteString("\n## Summaries by source directory\n\n| Directory | Functions | Prose summary | Coverage | Missing | Weak names | Generic names |\n|---|---|---|---|---|---|---|\n")
	for _, name := range sortedKeys(families) {
		group := families[name]
		b.WriteString(tableRow(name, fmt.Sprint(group.functions), fmt.Sprint(group.prose), percent(group.prose, group.functions),
			fmt.Sprint(group.functions-group.prose), fmt.Sprint(group.weakNames), fmt.Sprint(group.genericNames)))
	}

	type gap struct {
		subsystem        string
		functions, prose int
	}
	var gaps []gap
	for _, subsystem := range vault.subsystems() {
		functions := vault.subsystemFunctions(subsystem)
		g := gap{subsystem: subsystem, functions: len(functions)}
		for _, f := range functions {
			if proseSummary(f.summary) {
				g.prose++
			}
		}
		if g.prose < g.functions {
			gaps = append(gaps, g)
		}
	}
	sort.SliceStable(gaps, func(i, j int) bool {
		return gaps[i].functions-gaps[i].prose > gaps[j].functions-gaps[j].prose
	})
	b.WriteString("\n## Subsystems missing the most summaries\n\n| Subsystem | Functions | Missing |\n|---|---|---|\n")
	for position, g := range gaps {
		if position == 40 {
			break
		}
		b.WriteString(tableRow(vault.subsystemLink(g.subsystem), fmt.Sprint(g.functions), fmt.Sprint(g.functions-g.prose)))
	}

	fmt.Fprintf(&b, "\n## Function names with an address (%d)\n\n", len(addressNamed))
	b.WriteString("| Function | Size | Called by | Address used by | Summary |\n|---|---|---|---|---|\n")
	for _, f := range addressNamed {
		b.WriteString(tableRow(vault.functionLink(f.name), fmt.Sprint(f.instances[0].size), fmt.Sprint(len(f.calledBy)),
			fmt.Sprint(len(f.referencedBy)), firstSentence(f.summary)))
	}
	fmt.Fprintf(&b, "\n## Numbered names whose summary does not name the sibling (%d)\n\n", len(unjustified))
	b.WriteString(joinFunctionLinks(vault, unjustified) + "\n")
	fmt.Fprintf(&b, "\n## Header blocks whose prototypes mix subsystems (%d)\n\n| Header | Block | Subsystems |\n|---|---|---|\n", len(mixed))
	for _, use := range mixed {
		b.WriteString(tableRow(vault.headerLink(use.header), "`/* "+use.label+" */`", strings.Join(use.subsystems, ", ")))
	}
	var missing []string
	for _, use := range uses {
		if !use.purpose {
			missing = append(missing, fmt.Sprintf("%s `/* %s */`", strings.TrimPrefix(use.header, "include/fft/"), use.label))
		}
	}
	fmt.Fprintf(&b, "\n## Header blocks without a purpose comment (%d)\n\n%s\n", len(missing), strings.Join(missing, " · "))
	fmt.Fprintf(&b, "\n## QUIRKS.md entries without a link (%d)\n\n", len(unlinked))
	for _, quirk := range unlinked {
		fmt.Fprintf(&b, "- %s, line %d: %s\n", vault.quirkSectionLink(quirk.section), quirk.line, quirkExcerpt(quirk.text))
	}
	b.WriteString("\n## How the numbers are measured\n\n")
	b.WriteString("- A **prose summary** is the block comment directly above the definition, starting with a capital letter (not an address note).\n")
	b.WriteString("- **Weak names** are parameters and block-level locals named like MIPS registers (`a0`, `t1`, `s2`), placeholders (`unk*`, `var`, `arg1`, `local2`, `field0`, `val`, `tmp`, `temp2`) or single letters other than `i j k n x y z w h u v r g b`. **Generic names** (`value`, `data`, `ptr`, `temp`, `buf`, `buffer`, optionally numbered) are counted apart: fine in a short function, often hiding a better name.\n")
	b.WriteString("- A **numbered name** ends in `_N`; its summary justifies the number when it names the base (the sibling family).\n")
	b.WriteString("- A header block has a **purpose comment** when a standalone block comment (followed by a blank line) comes right after its `/* label */` line; it **mixes subsystems** when its prototypes carry more than one module-plus-word prefix.\n")
	return b.String()
}

func joinFunctionLinks(vault *mapVault, functions []*mapFunction) string {
	names := make([]string, len(functions))
	for i, f := range functions {
		names[i] = f.name
	}
	if len(names) == 0 {
		return "None."
	}
	return joinLinks(names, vault.functionLink)
}

// functionsTSV is a flat index of every function, for grep and for tools.
func (vault *mapVault) functionsTSV() string {
	var b strings.Builder
	b.WriteString("name\tmodule\taddress\tsize\tsubsystem\theader\tlabel\tsource\tprose_summary\tcalls\tcalled_by\tsummary\n")
	clean := strings.NewReplacer("\t", " ", "\n", " ")
	for _, name := range sortedKeys(vault.index.functions) {
		f := vault.index.functions[name]
		first := f.instances[0]
		fmt.Fprintf(&b, "%s\t%s\t%s\t%d\t%s\t%s\t%s\t%s\t%t\t%d\t%d\t%s\n", f.name, first.module, hex32(first.addr), first.size,
			f.subsystem, f.prototype.header, f.prototype.label, f.source, proseSummary(f.summary), len(f.calls), len(f.calledBy),
			clean.Replace(firstSentence(f.summary)))
	}
	return b.String()
}

// mapBases are the Bases views written into bases/ on every run.
var mapBases = map[string]string{
	"Functions.base": `filters: 'type == "function"'
views:
  - type: table
    name: "All functions"
    order: [file.name, module, subsystem, size, prose_summary, called_by, quirks, weak_locals]
  - type: table
    name: "Without a summary"
    filters: 'prose_summary == false'
    groupBy:
      property: subsystem
      direction: ASC
    order: [file.name, module, size, called_by]
  - type: table
    name: "Names with an address"
    filters: 'address_name == true'
    order: [file.name, module, size, called_by, referenced_by]
  - type: table
    name: "Numbered names to justify"
    filters: 'numeric_suffix == true && suffix_justified == false'
    order: [file.name, module, subsystem]
  - type: table
    name: "Weak local names"
    filters: 'weak_locals > 0'
    groupBy:
      property: subsystem
      direction: ASC
    order: [file.name, weak_locals]
  - type: table
    name: "With quirks"
    filters: 'quirks > 0'
    order: [file.name, module, quirks, debt]
  - type: table
    name: "With debt comments"
    filters: 'debt.length > 0'
    order: [file.name, module, debt]
  - type: table
    name: "No C caller"
    filters: 'called_by == 0 && referenced_by == 0'
    groupBy:
      property: module
      direction: ASC
    order: [file.name, subsystem, size]
`,
	"Types.base": `filters: 'type == "type"'
views:
  - type: table
    name: "All types"
    groupBy:
      property: header
      direction: ASC
    order: [file.name, kind, header_label, users]
  - type: table
    name: "Unused by name"
    filters: 'users == 0'
    order: [file.name, kind, header]
`,
	"Globals.base": `filters: 'type == "global"'
views:
  - type: table
    name: "All globals"
    groupBy:
      property: header
      direction: ASC
    order: [file.name, header_label, modules, users]
  - type: table
    name: "Unused by name"
    filters: 'users == 0'
    order: [file.name, header, modules]
`,
}

// mapObsidianSeed is written to .obsidian/ only when the vault has none, so
// the reader's own settings are never replaced. Core features only.
var mapObsidianSeed = map[string]string{
	"templates.json": `{
  "folder": "docs/templates"
}
`,
	"app.json": `{
  "readableLineLength": false,
  "showLineNumber": true,
  "alwaysUpdateLinks": false
}
`,
	"core-plugins.json": `{
  "file-explorer": true,
  "global-search": true,
  "switcher": true,
  "graph": true,
  "backlink": true,
  "canvas": true,
  "outgoing-link": true,
  "tag-pane": false,
  "properties": true,
  "page-preview": true,
  "daily-notes": false,
  "templates": true,
  "note-composer": false,
  "command-palette": true,
  "slash-command": false,
  "editor-status": true,
  "bookmarks": true,
  "markdown-importer": false,
  "zk-prefixer": false,
  "random-note": false,
  "outline": true,
  "word-count": false,
  "slides": false,
  "audio-recorder": false,
  "workspaces": false,
  "file-recovery": false,
  "publish": false,
  "sync": false,
  "bases": true,
  "webviewer": false
}
`,
	"graph.json": `{
  "collapse-filter": false,
  "search": "-path:functions -path:types -path:globals",
  "showTags": false,
  "showAttachments": false,
  "hideUnresolved": true,
  "showOrphans": false,
  "collapse-color-groups": false,
  "colorGroups": [
    {"query": "path:functions/battle", "color": {"a": 1, "rgb": 14701138}},
    {"query": "path:functions/world", "color": {"a": 1, "rgb": 5431378}},
    {"query": "path:functions/wldcore", "color": {"a": 1, "rgb": 11657298}},
    {"query": "path:functions/main", "color": {"a": 1, "rgb": 5395026}},
    {"query": "path:functions/event", "color": {"a": 1, "rgb": 14725458}},
    {"query": "path:functions/effect", "color": {"a": 1, "rgb": 11621088}},
    {"query": "path:functions/open", "color": {"a": 1, "rgb": 5431512}},
    {"query": "path:functions/psyq", "color": {"a": 1, "rgb": 9474192}}
  ],
  "collapse-display": false,
  "showArrow": true,
  "textFadeMultiplier": 0,
  "nodeSizeMultiplier": 1,
  "lineSizeMultiplier": 1,
  "collapse-forces": true,
  "centerStrength": 0.5,
  "repelStrength": 10,
  "linkStrength": 1,
  "linkDistance": 250,
  "scale": 1,
  "close": true
}
`,
}
