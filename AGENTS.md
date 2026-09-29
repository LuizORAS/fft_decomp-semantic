# AGENTS.md

A byte-exact matching decompilation of the USA PlayStation release of *Final
Fantasy Tactics* (`SCUS-94221`). Every game function is C that compiles to the
original bytes. The work now is making that C better (names, types, structure)
without changing a single byte.

## Linked libraries

Linked Psy-Q, runtime/startup and Suzuki library reconstructions are normal
repository source. Keep public SDK entry-point names, use `include/psx/`, and
put one reconstructed function per file under `src/psyq/<library>/`. Register
it in the original module's `target/` configuration with an explicit `source:`
path. Existing LIBGS, LIBPRESS and Suzuki functions stay in their current
module directories unless a separate change moves them coherently.

Library C has the same byte-exact requirements as game C. Every completed
function must match all original bytes; unmatched drafts stay in ignored
`build/`. Generate hashes only from original bytes through `make checksums`.
Never weaken checks, accept mismatches, or change a hash to bless edited code.
Use `make library-diff ARGS="--name=NAME --source=build/.../NAME.c
--addr=0xADDRESS --size=BYTES"` for scratch candidates before integration; it
uses the normal historical compiler pipeline without modifying target metadata.

Library headers and sources must never include `fft/` game headers. Move
shared library declarations to the library that logically owns them; game
headers may consume those declarations. Startup declares the program's `main`
as an external runtime contract. Keep ordinary declarations and driver
internals together; isolate or localize architectural register views so an
ordinary include cannot reserve CPU registers in unrelated callers.

Keep useful behavior and matching constraints in source comments and
`QUIRKS.md`, not research diaries or duplicate inventories. Retain a register
pin or empty assembly barrier only when it implements a real architectural ABI
or its removal changes the original bytes. Re-measure speculative workarounds
and remove those that prove unnecessary. Document uncertainty honestly.

These files implement the original PS1 behavior. BIOS firmware remains
external; linked BIOS veneers and hardware helpers are reconstructed here.
Game media, SDK archives/objects, extracted binaries and copied table dumps
remain excluded. Never modify `meta/`.

## Target

- Input: `scus-94221.bin` at the repo root (raw Mode 2/2352, 541,315,152
  bytes, SHA-256 `29e63bb6d2e7b842fe87f7c0ee03b7076681c34bcc47095cca27ec53236bf4f9`).
- Output: `build/disc/output-scus-94221.bin` + `.cue`, byte-identical to the
  input. These constants live in `tools/target.go`.
- Never add, stage or embed game media, extracted files, build output or
  proprietary SDK material. `meta/` is the user's private reference folder:
  never modify it.

## Layout

| Path | Contents |
|---|---|
| `src/<module>/` | One C file per function, named after it |
| `src/psyq/<library>/` | Byte-exact library reconstructions, registered with explicit `source:` paths in their original module |
| `include/fft/` | One header per module (`main.h`, `battle.h`, `world.h`, `wldcore.h`, `open.h`, `effect.h`, `event_<overlay>.h`) plus shared type headers (`thread.h`, `gfx.h`, `data.h`, `unit.h`, `map.h`, `menu.h`, `script.h`) |
| `include/psx/` | Psy-Q SDK, library and PS1 hardware declarations (keep SDK names and signatures) |
| `target/*.yaml` | Every disc module: functions with hashes, data names, libraries, regions |
| `tools/` | Go tooling (its own module), rebuilt from the working tree on every `make` call |
| `QUIRKS.md` | Index of retail bugs and code that looks wrong on purpose |
| `build/` | Ignored: extracted disc, caches, outputs, scratch work |

| Module | File | Load | Role |
|---|---|---|---|
| main | `SCUS_942.21` | `0x80010000` | Resident executable (`load: 0x8000f800` covers the 0x800-byte header) |
| battle | `BATTLE.BIN` | `0x80067000` | Battle engine |
| wldcore | `WORLD/WLDCORE.BIN` | `0x80067000` | World core, runs with WORLD |
| world | `WORLD/WORLD.BIN` | `0x800e0000` | World map, menus, event scripts |
| opening | `OPEN/OPEN.BIN` | `0x80067000` | Opening and title |
| event-* | `EVENT/*.OUT` | `0x801bf000`… | Menu screens run with BATTLE (`target/event.yaml`) |
| effect-* | `EFFECT/E*.BIN` | `0x801c2500` | 110 ability effects run with BATTLE (`target/effect.yaml`) |

File offset = address − `load`. Linked library ranges are declared under
`libraries:`. The linked Psy-Q and startup functions, LIBGS (WORLD), LIBPRESS
(OPENING), and Suzuki sound driver are reconstructed C. This includes the four
Suzuki heap routines at `0x8001423c`–`0x8001442c`, which use the original
`-G8` profile and `$gp` binding. Original padding, tables and executable
patch templates remain data in their original modules. `target/*.yaml` is
authoritative for each function's source, address, size, profile and hash.

## Commands

```sh
make bootstrap             # build the Docker image (extracts the BIN if present)
make validate [MODULE=x]   # compile and compare with target/ hashes; no BIN needed
make build [MODULE=x]      # full byte-exact build and disc (needs the BIN)
make extract               # extract the BIN (build/checksums/diff do it on demand)
make checksums             # refresh hashes from the ORIGINAL bytes only
make check-config          # validate target/, sources and declarations
make config-fmt            # rewrite target/*.yaml in canonical form
make declarations          # report conflicting declarations in detail
make symbols ACTION=rename-function|rename-global ARGS="--old A --new B"
make diff FUNC=f           # compare one function with the original bytes (needs the BIN)
make library-diff ARGS="..." # compare a scratch library candidate without registering it
make permute FUNC=f        # run decomp-permuter on it [DURATION=300 JOBS=4]
make test                  # vet and test the Go tooling
make fmt                   # clang-format src/ and include/ in place
make fmt-check             # fail on any formatting drift
make shell                 # interactive shell in the image
make run                   # build and launch PCSX-Redux (PCSX_REDUX=/path on Linux)
make compile-commands      # editor compile_commands.json (host clang)
```

`MODULE` takes an id (`battle`, `event-attack`) or an alias (`attack`,
`bunit`, `equip`, `jobstts`); for `validate` it may be a list, and `event` or
`effect` selects every such module. `diff` and `permute` need it only when
several modules define `FUNC`. Both compile exactly as `validate` does. Every change
ends with `make validate` passing. Before handing off, also run `make build`
(disc exact), `make check-config` and `make fmt-check`.

Every target runs in a fresh `docker run --rm` container of the pinned
`Dockerfile` image as the host user, with the checkout mounted at `/work`;
the image is tagged by a checksum of its inputs, built on first use (its last
layer proves every tool runs) and rebuilt by `make bootstrap`. It holds the
old GCC `cc1` binaries (native on x86-64, through `qemu-i386` elsewhere), maspsx, mipsel binutils and
`cpp`, mkpsxiso, asm-differ (`make diff` listings), decomp-permuter, Go and
clang-format 22.1.8. CI (`.github/workflows/check.yml`) runs `fmt-check`,
`check-config` and `test` in one job and `validate` in four module shards,
without the BIN. Optional overrides: `JOBS`, `CACHE=0`, `WORK_DIR`,
`REPORT_ALL=1`, `DURATION` (see the Makefile).

## target/*.yaml

One YAML document per module; `event.yaml` and `effect.yaml` hold many,
separated by `---`. `make config-fmt` defines the canonical form (unquoted hex
addresses, decimal sizes, one flow mapping per line, sorted rows) and
`make check-config` rejects anything else.

- Header: `module`, `file`, `lba`, `size`, `sha256`, `load`, `source_dir`,
  `profile` (module default), `links` (modules resident alongside).
- `functions`: `{addr, size, name, hash}`, plus `profile`, `rodata` or `asm`
  only when needed. `hash` is the first 16 hex digits of the SHA-256 of the
  original bytes; only `make checksums` writes it.
- `data`: named addresses with no reconstruction (globals, library entry
  points). `imports`: names from overlays that linking cannot otherwise
  reach. `overrides`: per-function bindings. `regions`: bytes deliberately
  left original, with the reason.
- Linking one function binds each name it uses from its own module, then
  each `links` module in order, then main.

To add a function: write `src/<module>/<name>.c`, add its `functions` row and
any new `data` rows, then `make config-fmt`, `make checksums`,
`make validate MODULE=…`, `make build`. Rename with `make symbols` so sources,
headers, `target/` and the file name change together.

## Compiler profiles

Default: GCC 2.6.3 `-O2 -G0`, soft-float R3000, ASPSX 2.34 via maspsx. Other
profiles are used only where a function matches no other way:

- `-O1` and `-O0`: parts of WORLD and the CARD screen.
- ASPSX 2.21: WLDCORE (its module default), about 60 OPENING functions and a
  few in BATTLE and MAIN.
- `gcc-2.7.2_O2_aspsx-2.21`: WLDCORE and OPENING functions that need GCC 2.7
  code: the separate `mfhi` in constant divisions, or a `lw ra` epilogue with
  its delay slot filled.
- `_divcheck`: division with the zero/overflow `break` checks.
- `gcc-2.6.3_O2_G8_aspsx-2.34`: the four Suzuki CPU heap routines require
  `$gp`-relative small data with `$gp = 0x800329bc`. The measured assembler
  load-delay correction applies only to this profile; the default remains G0.

Never add a profile or move a function between profiles without a specific
function that matches only that way.

## Code rules

- Byte-exact always. Never weaken a check or edit a hash to accept a mismatch.
- C only. The single `.s` file (`main_restore_game_loop_stack_pointer.s`) is
  an ABI boundary. Instruction-emitting inline asm remains in the hand-written
  thread/copy helpers and the PS1 hardware/ABI helpers in `include/psx/`.
  Library algorithms stay C; each hardware instruction or register constraint
  must have a measured PS1 dependency or byte-exact ABI reason.
- Typed access: structs and named fields, never raw offsets such as
  `*(s16*)((u8*)x + 0x218)`. Add a missing field to its shared header with a
  provisional name. A raw offset, cast, register pin or empty asm barrier stays
  only when the clean spelling provably changes the bytes, with a one-line
  comment saying so.
- Names carry the module prefix (`battle_`, `world_`, `wldcore_`, `open_`,
  `main_`, `<overlay>_`; globals `g_battle_`, …). Unknowns stay
  `func_ADDRESS` or `D_ADDRESS` until the code supports a name. Unnamed
  struct members are `_unknown_XX` (code accesses it, meaning unknown),
  `_unused_XX` (no source accesses it by name) or `_padding_XX` (alignment
  or size filler, with an inline comment saying which), where XX is the
  hex offset in the member's own struct, zero-padded to the same width
  throughout the struct. `make check-config` fails on any access to an
  `_unused_` or `_padding_` member. snake_case; `_t` struct and `_e` enum typedefs; uppercase enum values;
  `_init_`, not `_initialize_`. `include/psx/` keeps SDK names. Header guards
  use descriptive header/path names without an `FFT_` prefix. A library
  header groups its SDK interface and reconstructed driver internals together;
  register-reserving ABI helpers stay in focused separate headers.
  In partial SDK/BIOS views, a field absent from this game's accesses remains
  `_unknown_XX`; use `_unused_XX` only when broader library/BIOS evidence proves
  it unused. Proven register spacing and alignment remain `_padding_XX`.
- Fixed point is 1.3.12 / 20.12 with 1.0 = `ONE` (4096, `psx/libgte.h`), also
  one full rsin/rcos turn. Write `ONE` for 1.0; `>> 12` rescales after a
  multiply.
- One declaration per symbol, in its header. `make check-config` fails on
  conflicting declarations. Globals and prototypes go in the header of their
  name's module prefix. A type goes in the most specific module header that
  every user can see: `battle.h` for BATTLE and its overlays (whose headers
  include it), `world.h` for WORLD, WLDCORE and OPENING, and a shared type
  header only when both families use it. Two exceptions: `main_` types stay
  in `main.h`, and MAIN-only table or unit records (item, ability, job,
  party, ENTD) sit with their siblings in `data.h` and `unit.h`. A `volatile`
  or otherwise one-function view of a global stays local to that source
  file, with its reason. A source file includes its own
  module header, plus another module's header when it calls into that module
  (MAIN and WORLD code calling BATTLE functions, for example). `main.h`
  includes every shared header. Each header groups its declarations under
  short lowercase subsystem labels (`/* ai */`), types first, then globals,
  then functions.
- Comments: an optional short summary above a function, then only the
  rationale needed to read the code. State facts, not history. Record retail
  bugs and caller/callee disagreements with one line in `QUIRKS.md`.
- Run `make fmt` after editing C or headers, then `make validate`: cc1 emits
  line notes, so formatting can change the bytes.

## Matching levers

When a cleanup changes the bytes, look here first.

- Stores ignore signedness; loads, compares, shifts and divisions don't. To
  get `lh` from a `u16` (or `lhu` from an `s16`), cast the lvalue
  (`*(s16*)&g`), not the value.
- A narrow prototype adds `andi` or `sll`/`sra` at callers; widening removes
  them. Function-pointer casts emit nothing.
- A fresh `lui` for each neighbouring global means separate symbols. A struct
  view gives GCC one base register and reshapes the body; keep the aliases.
- Raw offset versus field access changes scheduling (`MEM_IN_STRUCT_P`):
  re-measure after converting.
- Two single-bit tests on one `u16` (`(f & 0x100) && !(f & 0x10)`) merge into
  one word `lw`+`andi` test.
- One variable in two roles across a call forces a callee-saved register;
  split or merge variables before trying priority tricks. Read conflicts
  with `cc1 -da` (`.greg`).
- Single-set constants and temps schedule late (birthing boost); a multi-set
  variable of the same width moves them earlier.
- Any store through a pointer drops CSE of cached field loads; one store
  placed before a reload reproduces a target reload. To force a reload of
  one field, use a `volatile` lvalue on that field only.
- `count = obj->count;` before the loop is one instruction shorter than
  `i < obj->count`.
- One `volatile` access in the fall-through block stops reorg filling a delay
  slot from past the join.
- `if (x) continue; break;` gives `bnez x,next; j out`; `if (!x) break;` gives
  the inverted branch.
- A prologue that copies `$a2` before `$a1` means the loop walks a local copy
  (`p = param;`).
- `switch` emits all compares (a tree under 5 cases, a jump table above), then
  the bodies; an `if` chain puts each body between compares. Write case bodies
  in the target's physical order.
- Two stores sharing one `la` base (`sw 0(v1)` / `sw -4(v1)`) point to `-O1`:
  `-O2`'s second CSE pass folds them back to absolute stores.
- A frame bigger than its spills means unused locals in the original;
  declaration order sets slot order.

Tolerated residue (zero-instruction only, each with a comment): the tied
operand barrier `__asm__("" : "=r"(x) : "0"(x))`, an empty `asm volatile`, a
`register … __asm__("$N")` pin, a dead local kept for frame size, and a
second link name for one address. Each is debt: try removing it, and keep it
only if removal changes the bytes.

## Improving the code

Useful work, each step verified with `make validate`:

- Name the remaining `D_` globals, `func_` functions and `_unknown_`
  members from what the code does with them, and examine `_unused_` members
  (they may still be touched by whole-struct copies or another view).
- Remove register pins, asm barriers, casts and raw offsets where a natural
  spelling matches (about 180 files still pin a register).
- Replace `goto` (about 300 remain) with the loops, `if`/`else` and `switch`
  the compiler actually produced.
- Fold adjacent globals the code proves are one record; keep separate
  symbols where the original reloads each address.
- Give `s32` handles and `void*` parameters their real types.

## Working with agents

For large tasks, split the work into disjoint files or modules and give each
worker its own scratch path under `build/`. One lead edits shared headers and
`target/*.yaml`, integrates results and runs the final `make build`. Workers
report the address range, match result and any proposed header change. Never
commit; the user commits.
