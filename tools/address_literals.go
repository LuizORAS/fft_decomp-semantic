// address_literals.go keeps RAM addresses out of code. A number in the game's
// RAM above the kernel is an address: one inside a module moves with it, so
// code names its symbol; a fixed area outside every module image (a buffer
// OPEN or WLDCORE keeps past the images) is a #define in a header, where the
// memory map can find it. check-config fails on any other such number in the
// C sources and headers; the hand-written .s files have their own check
// (validateAssemblySource). Comments and string literals do not count.
package main

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strconv"
)

// addressLiteralLow and addressLiteralHigh bound the numbers that are game RAM
// addresses: the same scope as reloc-check's data words. Below lies the kernel,
// where 0x80000000 is the sign bit of masks and flags.
const (
	addressLiteralLow  = relocScopeLow
	addressLiteralHigh = relocScopeHigh
)

var (
	hexLiteral        = regexp.MustCompile(`\b0[xX]([0-9a-fA-F]+)[uUlL]*\b`)
	directiveIsDefine = regexp.MustCompile(`^[ \t]*#[ \t]*define\b`)
)

type addressLiteral struct {
	path  string
	line  int
	value uint32
}

func isAddressLiteral(text string) (uint32, bool) {
	match := hexLiteral.FindStringSubmatch(text)
	if match == nil || match[0] != text {
		return 0, false
	}
	value, err := strconv.ParseUint(match[1], 16, 64)
	return uint32(value), err == nil && value >= addressLiteralLow && value < addressLiteralHigh
}

// findAddressLiterals lists the RAM addresses written as numbers in the code
// of every .c and .h file under dirs, except a header's #define lines.
func findAddressLiterals(root string, dirs []string) ([]addressLiteral, error) {
	var found []addressLiteral
	for _, dir := range dirs {
		err := filepath.WalkDir(filepath.Join(root, dir), func(path string, entry fs.DirEntry, err error) error {
			if err != nil {
				return err
			}
			extension := filepath.Ext(path)
			if entry.IsDir() || (extension != ".c" && extension != ".h") {
				return nil
			}
			data, err := os.ReadFile(path)
			if err != nil {
				return err
			}
			relative, err := filepath.Rel(root, path)
			if err != nil {
				return err
			}
			relative = filepath.ToSlash(relative)
			source := string(data)
			tokens, err := tokenizeC(source)
			if err != nil {
				return fmt.Errorf("%s: %w", relative, err)
			}
			for _, token := range tokens {
				if value, ok := isAddressLiteral(token.text); token.kind == cNumber && ok {
					found = append(found, addressLiteral{relative, lineOf(source, token.start), value})
				}
			}
			// tokenizeC skips preprocessor lines: a header names a fixed area
			// with a #define, and any other directive line is code like the rest.
			for _, directive := range cDirectives(source) {
				if extension == ".h" && directiveIsDefine.MatchString(directive.text) {
					continue
				}
				for _, text := range hexLiteral.FindAllString(directive.text, -1) {
					if value, ok := isAddressLiteral(text); ok {
						found = append(found, addressLiteral{relative, lineOf(source, directive.start), value})
					}
				}
			}
			return nil
		})
		if err != nil && !errors.Is(err, fs.ErrNotExist) {
			return nil, err
		}
	}
	sort.Slice(found, func(i, j int) bool {
		if found[i].path != found[j].path {
			return found[i].path < found[j].path
		}
		return found[i].line < found[j].line
	})
	return found, nil
}

// validateAddressLiterals fails when code writes a RAM address as a number
// outside a header's #define.
func validateAddressLiterals(root string) error {
	found, err := findAddressLiterals(root, declarationScanDirs)
	if err != nil {
		return err
	}
	if len(found) == 0 {
		return nil
	}
	sites := make([]string, len(found))
	for index, literal := range found {
		sites[index] = fmt.Sprintf("%s:%d 0x%08x", literal.path, literal.line, literal.value)
	}
	return fmt.Errorf("addresses: %d RAM addresses written as numbers in code; name the symbol, or for a fixed area outside the module images a #define in a header: %s",
		len(found), abbreviatedList(sites, 10))
}
