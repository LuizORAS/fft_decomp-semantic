package main

import (
	"fmt"
	"strings"
)

type shareAuditLocalRole struct {
	Role        string `json:"role"`
	Name        string `json:"name"`
	Kind        string `json:"kind"`
	Declaration string `json:"declaration"`
}

type shareAuditLocalNormalization struct {
	Status string                `json:"status"`
	Reason string                `json:"reason,omitempty"`
	Roles  []shareAuditLocalRole `json:"ordered_local_roles,omitempty"`
}

// Only simple parameters and outer-block automatic objects are eligible.
// This deliberately avoids scope resolution and preprocessing claims.
func (scanner *shareAuditScanner) normalizeLocalRoles(source string, all, parameters, body []cToken,
	bindings map[string]uint32, self string) (map[string]string, []string, *shareAuditLocalNormalization) {
	roles := map[string]string{}
	declarationStarts := map[string]int{}
	report := &shareAuditLocalNormalization{Status: "normalized"}
	reject := func(reason string) (map[string]string, []string, *shareAuditLocalNormalization) {
		return nil, nil, &shareAuditLocalNormalization{Status: "exact_fallback", Reason: reason}
	}
	if len(body) < 2 {
		return reject("unparsed_body")
	}
	for _, line := range strings.Split(source[body[0].start:body[len(body)-1].end], "\n") {
		if strings.HasPrefix(strings.TrimSpace(line), "#") {
			return reject("body_preprocessor")
		}
	}
	knownStart := func(tokens []cToken) bool {
		return len(tokens) > 0 && (scanner.nonTagTypes[tokens[0].text] || declarationQualifiers[tokens[0].text] ||
			isTagKeyword(tokens[0].text) || tokens[0].text == "static" || tokens[0].text == "typedef")
	}
	collect := func(statement []cToken, kind string) string {
		for _, token := range statement {
			if token.text == "static" || token.text == "extern" || token.text == "typedef" {
				return "nonautomatic_declaration"
			}
		}
		parts := splitTopLevel(statement, ",")
		if len(parts) == 0 || len(parts[0]) == 0 || !knownStart(parts[0]) {
			return "unknown_declaration_type"
		}
		_, begin, _ := parseDeclarationSpecifier(parts[0])
		parsed := parseDeclaratorList(statement, false)
		if len(parsed) != len(parts) {
			return "incomplete_declaration"
		}
		for index, part := range parts {
			declarator := part
			if index == 0 {
				declarator = part[begin:]
			}
			if equals := topLevelIndex(declarator, "="); equals >= 0 {
				if kind == "parameter" {
					return "parameter_initializer"
				}
				for _, token := range declarator[equals+1:] {
					if token.text == "{" || token.text == "}" {
						return "aggregate_initializer"
					}
				}
				declarator = declarator[:equals]
			}
			cursor := 0
			for cursor < len(declarator) && (declarator[cursor].text == "*" || declarationQualifiers[declarator[cursor].text]) {
				cursor++
			}
			if cursor >= len(declarator) || declarator[cursor].kind != cIdent || declarator[cursor].text != parsed[index].symbol {
				return "complex_declarator"
			}
			name := declarator[cursor].text
			if kind == "local" {
				declarationStarts[name] = declarator[cursor].start
			}
			cursor++
			for cursor < len(declarator) {
				if declarator[cursor].text != "[" {
					return "complex_declarator"
				}
				end := matchingToken(declarator, cursor)
				if end < 0 {
					return "complex_declarator"
				}
				for _, token := range declarator[cursor+1 : end] {
					if token.text == "(" || token.text == ")" || token.text == "{" || token.text == "}" {
						return "complex_array_extent"
					}
				}
				cursor = end + 1
			}
			if _, duplicate := roles[name]; duplicate {
				return "duplicate_or_shadowed_name"
			}
			if _, bound := bindings[name]; bound {
				return "linker_symbol_shadow"
			}
			if scanner.nonTagTypes[name] || name == self {
				return "type_or_self_collision"
			}
			role := fmt.Sprintf("$local%d", len(report.Roles))
			roles[name] = role
			report.Roles = append(report.Roles, shareAuditLocalRole{role, name, kind,
				strings.TrimSpace(source[part[0].start:part[len(part)-1].end])})
		}
		return ""
	}
	if len(parameters) == 0 {
		return reject("unprototyped_parameters")
	}
	if len(parameters) != 1 || parameters[0].text != "void" {
		for _, parameter := range splitTopLevel(parameters, ",") {
			if reason := collect(parameter, "parameter"); reason != "" {
				return reject(reason)
			}
		}
	}
	depth := 0
	controls := map[string]bool{"return": true, "if": true, "else": true, "for": true, "while": true,
		"do": true, "switch": true, "case": true, "default": true, "break": true, "continue": true, "goto": true}
	for index, token := range body {
		if token.text == "{" {
			depth++
		} else if token.text == "}" {
			depth--
		}
		if token.text == "for" && index+2 < len(body) && body[index+1].text == "(" && knownStart(body[index+2:]) {
			return reject("for_declaration")
		}
		if index == 0 || index+1 >= len(body) {
			continue
		}
		previous := body[index-1].text
		if previous != "{" && previous != ";" && previous != "}" && previous != ":" {
			continue
		}
		if !knownStart(body[index:]) {
			if token.kind == cIdent && !controls[token.text] && (body[index+1].kind == cIdent || body[index+1].text == "*") {
				return reject("unknown_declaration_start")
			}
			continue
		}
		if depth != 1 || previous == ":" {
			return reject("nested_or_labeled_declaration")
		}
		end := index
		nesting := 0
		for end < len(body) {
			if body[end].text == ";" && nesting == 0 {
				break
			}
			switch body[end].text {
			case "(", "[", "{":
				nesting++
			case ")", "]", "}":
				nesting--
			}
			end++
		}
		if end >= len(body) || nesting != 0 {
			return reject("unparsed_declaration")
		}
		if reason := collect(body[index:end], "local"); reason != "" {
			return reject(reason)
		}
	}
	for index, token := range body {
		member := index > 0 && (body[index-1].text == "." || body[index-1].text == "->")
		if start, local := declarationStarts[token.text]; local && !member && token.start < start {
			return reject("use_before_declaration")
		}
		if index+1 < len(body) && token.kind == cIdent && body[index+1].text == "(" &&
			!controls[token.text] && token.text != "sizeof" && (index == 0 || body[index-1].text != "." && body[index-1].text != "->") {
			if _, bound := bindings[token.text]; !bound {
				end := matchingToken(body, index+1)
				if end < 0 {
					return reject("unparsed_call")
				}
				for _, arg := range body[index+2 : end] {
					if _, local := roles[arg.text]; local {
						return reject("unknown_call_or_macro")
					}
				}
			}
		}
		if _, local := roles[token.text]; local && (index > 0 && isTagKeyword(body[index-1].text) || index+1 < len(body) && body[index+1].text == ":") {
			return reject("tag_or_label_collision")
		}
	}
	for index, token := range all {
		if token.text != self || index+1 >= len(all) || all[index+1].text != "(" {
			continue
		}
		end := matchingToken(all, index+1)
		if end < 0 || end+1 >= len(all) || all[end+1].start != body[0].start {
			continue
		}
		begin := index
		for begin > 0 && all[begin-1].text != ";" && all[begin-1].text != "}" {
			begin--
		}
		header := []string{"$full_definition_header"}
		for headerIndex, part := range all[begin : end+1] {
			text := part.text
			if headerIndex == 0 || !isTagKeyword(all[begin+headerIndex-1].text) {
				if text == self {
					text = "$self"
				} else if role, ok := roles[text]; ok {
					text = role
				}
			}
			header = append(header, text)
		}
		return roles, header, report
	}
	return reject("unparsed_definition_header")
}
