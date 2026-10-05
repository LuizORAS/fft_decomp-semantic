# Codebase guide

How the decompiled *Final Fantasy Tactics* (`SCUS-94221`) fits together: what
is on the disc, how control moves between modules while the game runs, the
engine services every module uses, and where to look for a mechanic.

Related pages:
- [`AGENTS.md`](AGENTS.md): the working rules (layout, names, types, matching).
- [`QUIRKS.md`](QUIRKS.md): retail bugs and code that looks wrong on purpose.
- [`docs/`](docs/README.md): one page per game mechanic and guides for common
  changes.
- `make map`: generates a browsable page for every function, type and global
  (see [Finding things](#finding-things)).

## The disc

The game is one resident executable plus overlays that it loads from the CD
to fixed addresses. Each module has a configuration in [`target/`](target/)
and a source folder in [`src/`](src/). The configuration holds the address,
size and hash of every function, the names of its data, and the regions kept
as original bytes.

| Module | Disc file | Loaded at | What it runs |
|---|---|---|---|
| `main` | `SCUS_942.21` | `0x80010000`, resident | Boot, the top-level game loop, disc reads, memory, sound, pads, memory card, the shared data tables and the linked Sony libraries |
| `opening` | `OPEN/OPEN.BIN` | `0x80067000` | Opening movie, title screen and the ending |
| `wldcore` | `WORLD/WLDCORE.BIN` | `0x80067000` | World side: the menu stack, propositions and event-script opcodes |
| `world` | `WORLD/WORLD.BIN` | `0x800e0000` | World map, travel, formation, shops, and the world event-script interpreter |
| `battle` | `BATTLE.BIN` | `0x80067000` | Battles and the story scenes played on battle maps |
| `event-*` | `EVENT/*.OUT` | `0x801bf000` | Screens and loaders that BATTLE runs on demand: unit, equipment and job menus (`BUNIT`, `EQUIP`, `JOBSTTS`), the scenario loader (`ATTACK`) and others |
| `effect-*` | `EFFECT/E*.BIN` | `0x801c2500` | 110 ability effects, run with BATTLE |

Memory follows from these addresses:
- BATTLE, WLDCORE and OPEN share `0x80067000`, so only one of them is loaded
  at a time.
- WORLD sits above WLDCORE and runs with it.
- The event and effect overlays sit above BATTLE.

A function links against its own module, then the modules listed under
`links:` in its configuration, then MAIN. It never calls into a module that
is swapped out. A file offset is the address minus the module's `load`.

## Boot and the game loop

`main` ([`src/main/main.c`](src/main/main.c)) runs three steps:

1. **[`main_boot_run_startup`](src/main/main_boot_run_startup.c):**
   - clears the allocator table;
   - installs the VSync, DrawSync and CD callbacks;
   - resets the GPU, pads, SPU and CD;
   - draws the SCEA and Squaresoft logos;
   - opens the memory card events and the generic sound effects;
   - loads the zodiac frame and seeds `rand` with 1.
2. **Saves its stack pointer**, which
   `main_restore_game_loop_stack_pointer.s` (the one assembly file) restores on a
   reset (`main_system_reset_game`) before jumping back to the top of the game
   loop.
3. **Enters [`main_system_run_game_loop`](src/main/main_system_run_game_loop.c)**,
   which never returns:

```
forever:
    OPEN.BIN: opening and title         main_overlay_exec_open_bin_main_loop
    repeat:
        WLDCORE + WORLD: world map      main_overlay_exec_wldcore_and_world_bin
        BATTLE.BIN: battle or scene     main_overlay_exec_battle_bin
                                        battle_state_run_game_loop
    until a reset (back to OPEN) or the ending
    ending: OPEN.BIN ending, one last BATTLE.BIN run, then reset
```

Three globals steer the loop:
- **`g_main_system_go_straight_to_battle`:** skips the world map after the
  title.
- **`g_main_system_frontend_world_result`:** a world result of 5 returns to
  OPEN without resetting the game state.
- **`g_main_system_game_flow_state`:** 2 resets the game, 3 plays the ending.

Each overlay then runs its own loop. BATTLE runs a state machine
(`battle_state_*`, about 100 functions) and a turn clock (`battle_turn_*`); the
[Battle flow](docs/mechanics/Battle%20flow.md) page walks through both. WLDCORE
starts at `wldcore_entrypoint` and returns the world result to MAIN.

## Engine services

The [Engine core](docs/mechanics/Engine%20core.md) page covers these services
in more depth, with the memory map and where to change each one.

### Disc reads

[`main_file_load_data_from_disc`](src/main/main_file_load_data_from_disc.c)
works like this:
- it takes an absolute sector, a length in sectors and a destination;
- it advances an asynchronous CD read once per frame
  (`main_file_poll_load`, then `VSync`) and keeps the loading indicator
  current.

The sectors are constants in the code:
- 1000 for BATTLE.BIN, in `main_overlay_exec_battle_bin`;
- 84041 (`0x14849`) for WLDCORE.BIN and 84261 (`0x14925`) for WORLD.BIN, in
  `main_overlay_exec_wldcore_and_world_bin`.

So a file that moves or grows on the disc breaks these reads. This is one
reason today's build cannot change file sizes (see
[What can change today](#what-can-change-today)).

### Memory

Overlays load at `g_main_heap_low_overlay_load_address`: BATTLE.BIN gets
everything up to `g_main_heap_high_overlay_load_address`.

The game allocator ([`main_heap_alloc`](src/main/main_heap_alloc.c)) manages
the area above that, in 64 cells of 2 KB:
- it takes the first run of free cells that fits;
- it tags the cells with a new allocation id;
- it returns 0 when nothing fits.

The sound data has its own table (`main_heap_alloc_smd`), and the Suzuki
sound driver keeps a separate CPU heap.

### Threads

BATTLE and WORLD each schedule cooperative native threads:
- **Slots:** `0x400` bytes each, with about 900 bytes of stack
  (`native_thread_t` in [`include/fft/thread.h`](include/fft/thread.h)).
  Slot 0 is the overlay's main loop; BATTLE runs threads in slots 1–15 and
  WORLD in slots 1–16.
- **A frame:** the main loop yields once a frame
  (`battle_script_run_event_frame`, `world_script_run_frame`). The
  hand-written `battle_thread_yield` and `world_thread_yield` save the
  thread's registers in its slot and resume the next running slot, returning
  to slot 0 after the last. So every running thread advances once a frame,
  and `*_thread_wait_frames(n)` waits n frames.
- **Starting and identifying:** `*_thread_resolve_id` finds a free slot,
  `*_thread_set_parameters` passes the inputs, `battle_thread_start` and
  `world_thread_start` start the function, and a `task_id` lets other threads
  find it.
- **Large calls:** a thread runs disc reads and overlay loads through
  `*_thread_call_on_main_stack`, on the main loop's stack.
- **What runs this way:** menu cursors, event unit moves and dialogue text.

### Data tables

MAIN holds the shared tables that both sides read:
- abilities, items, jobs, skillsets and statuses
  ([`include/fft/data.h`](include/fft/data.h));
- unit records ([`include/fft/unit.h`](include/fft/unit.h)).

The repository holds their types only. The bytes stay on the disc, because
game data is never committed.

### Event scripts

Story scenes are bytecode, run by three sets of code:
- the BATTLE interpreter (`battle_script_*`);
- the WORLD interpreter (`world_script_*`);
- WLDCORE's opcode handlers (`wldcore_opcode_*`).

[`include/fft/script.h`](include/fft/script.h) declares the bytecode and the
script variables that both interpreters share. The game loop itself writes
two of them before the ending.

### Sound, graphics and libraries

- **Sound:** MAIN's `main_sound_*` controls it, and the Suzuki driver
  (`main_smd_*`, `Suzuki*`) plays it.
- **Graphics:** they go through the Psy-Q GPU and GTE libraries
  ([`src/psyq/`](src/psyq/)), in fixed point where `ONE` (4096) is 1.0.
- **Other libraries:** WORLD links LIBGS, and OPEN links LIBPRESS for the
  movies.

All of these libraries are reconstructed C, matched like the game code.

## Source layout and names

- **Files:** one function per file, at `src/<module>/<name>.c`. Sony
  libraries live under `src/psyq/<library>/`.
- **Headers:** one per module in [`include/fft/`](include/fft/), plus shared
  type headers. Each header groups its declarations under short subsystem
  labels such as `/* ai */`.
- **Names:** a module prefix, a subsystem word, then the action:
  `battle_move_...`, `world_shop_...`. The prefix and subsystem word form
  the *code subsystem* that the map groups functions by. Unknowns stay
  `func_ADDRESS` and `D_ADDRESS`, and struct members of unknown meaning stay
  `_unknown_XX`.
- **Copies:** overlays repeat code. 115 groups of functions are byte for byte
  identical (same hash) under different names in different modules.

[`AGENTS.md`](AGENTS.md) has the full rules.

## Finding things

- **The vault:** `make map` writes an Obsidian vault to `build/map/`; open
  that folder as a vault. On Windows, Obsidian cannot open a vault inside
  WSL: add `MAP_EXPORT=/mnt/d/fft-map` and open that copy.
  - **Home** lists modules, code subsystems, mechanics and reports.
  - **Function pages:** each one shows the summary, the code, callers and
    callees, the globals and types it uses, and its `QUIRKS.md` entries.
  - **Bases views:** sort and filter every function.
  - **`build/map/functions.tsv`:** has one line per function, for grep
    (`grep -i formula build/map/functions.tsv`).
- **Mechanics and guides:** [`docs/mechanics/`](docs/README.md) pages give
  the purpose, data, flow and "where to change" of each mechanic, with the
  list of functions in its scope: [Battle flow](docs/mechanics/Battle%20flow.md),
  [Movement](docs/mechanics/Movement.md) and
  [Engine core](docs/mechanics/Engine%20core.md) so far. The guides walk
  through common changes, such as the
  [Movement rules](docs/guides/Movement%20rules.md).
- **Calls through tables:** they do not appear in C, so about 1300 functions
  have no C caller. Their pages say so.
- **Debugging:** `make map` also writes PCSX-Redux symbol maps to
  `build/symbols/`, so the debugger shows these names. `make run` launches
  the emulator.
- **Upstream names:** the vault's Upstream renames page translates
  upstream's names to this fork's, by address.
  [`docs/upstream-renames.tsv`](docs/upstream-renames.tsv) is its committed
  copy, which GitHub shows as a searchable table.

## What can change today

Every function is compiled and compared with its original bytes, and
`make build` reproduces the original disc. There is no switch yet for a build
that differs. So today the tree supports:
- renames, types, comments and documentation;
- rewrites that keep the bytes.

Gameplay changes wait for two later steps:
- **The shiftable build** (planned after this one):
  - each module links as a whole, so functions can grow and move;
  - data gets symbols;
  - the disc sectors above come from the disc layout instead of constants.
- **Build 3:** the `QUIRKS.md` entries and the "Type debt / Port debt"
  comments become optional fixes. The vault's Build 3 backlog page collects
  them.
