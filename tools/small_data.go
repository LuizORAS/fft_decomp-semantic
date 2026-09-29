package main

import (
	"bufio"
	"bytes"
	"os"
	"regexp"
	"strconv"
	"strings"
)

var smallDataExtern = regexp.MustCompile(`^\s*\.extern\s+([A-Za-z_][A-Za-z_0-9]*),\s*([0-9]+)\s*$`)
var smallDataLoad = regexp.MustCompile(`^\s*(?:lb|lbu|lh|lhu|lw)\s+(\$[A-Za-z_0-9]+),`)
var smallDataStore = regexp.MustCompile(`^\s*(?:sb|sh|sw)\s+(\$[A-Za-z_0-9]+),\s*([A-Za-z_][A-Za-z_0-9]*)\s*$`)

// maspsx does not classify .extern declarations as small-data entries when
// checking hazards. GNU as -G8 does: a symbolic store may be one instruction,
// with no intervening lui to cover the preceding load's delay. Restore that
// required nop only when the extern's declared size proves this expansion.
func smallDataLoadDelays(assembly []byte, limit int) []byte {
	small := make(map[string]bool)
	var lines []string
	scanner := bufio.NewScanner(bytes.NewReader(assembly))
	for scanner.Scan() {
		line := scanner.Text()
		lines = append(lines, line)
		if match := smallDataExtern.FindStringSubmatch(line); match != nil {
			size, err := strconv.Atoi(match[2])
			if err == nil && size > 0 && size <= limit {
				small[match[1]] = true
			}
		}
	}
	// Keep input intact if the scanner cannot represent a long inline macro.
	if scanner.Err() != nil {
		return assembly
	}
	var output strings.Builder
	for i, line := range lines {
		output.WriteString(line)
		output.WriteByte('\n')
		load := smallDataLoad.FindStringSubmatch(strings.SplitN(line, "#", 2)[0])
		if load == nil {
			continue
		}
		for _, next := range lines[i+1:] {
			instruction := strings.TrimSpace(strings.SplitN(next, "#", 2)[0])
			if instruction == "" || strings.HasPrefix(instruction, ".") || strings.HasSuffix(instruction, ":") {
				continue
			}
			store := smallDataStore.FindStringSubmatch(instruction)
			if store != nil && load[1] == store[1] && small[store[2]] {
				output.WriteString("nop # GPREL store consumes the previous load\n")
			}
			break
		}
	}
	return []byte(output.String())
}

func correctSmallDataLoadDelays(path string, limit int) error {
	assembly, err := os.ReadFile(path)
	if err != nil {
		return err
	}
	return os.WriteFile(path, smallDataLoadDelays(assembly, limit), 0o644)
}
