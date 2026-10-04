---
type: mechanic
tier: 1
scope:
  - main()
  - main_restore_game_loop_stack_pointer()
  - main_system
  - main_boot
  - main_overlay
  - main_file
  - main_heap
  - battle_heap
  - battle_thread
  - world_thread
---

# Engine core

## What it does in the game

What every scene stands on, though the player never sees it: booting the
console and showing the logos, the top-level loop that moves between the
title, the world map and battles, loading each part of the game from the
disc, sharing the 2 MB of RAM, and running many small tasks side by side every
frame (menu cursors, dialogue text, event actors).

## Data and structures

**Memory map.** MAIN stays resident; the overlays share fixed windows. The
words at the start of MAIN's data (`0x80010000`) hold the load addresses.

| Range | Holds |
|---|---|
| `0x80010000`– | MAIN (`SCUS_942.21`), with the SMD heap for sound files at `0x8004eb18` (16 cells of 2 KB) |
| `0x80067000`– | OPEN, WLDCORE or BATTLE, one at a time ([[g_main_heap_low_overlay_load_address]]) |
| `0x800e0000`– | WORLD, beside WLDCORE ([[g_main_heap_world_overlay_load_address]]) |
| `0x801bf000`– | EVENT and EFFECT overlays, beside BATTLE ([[g_event_overlay_load_address]]) |
| up to `0x801df000` | the battle heap, from the effect palette buffer ([[g_battle_heap_end_address]]) |
| `0x801df000`–`0x801ff000` | the game heap, 64 cells of 2 KB ([[g_main_heap_high_overlay_load_address]]); companion overlays load over it after reserving it |
| `0x801ff000`– | the main stack, which a deep WORLD frame can push into the heap's top cells |

**Records and globals.**
- Disc reads: [[main_file_load_descriptor_t]], one read in flight per
  descriptor; [[g_main_file_cd_state]] is the shared one, and
  [[g_main_file_still_loading]] is its state word under a second name.
- Heaps: [[g_main_heap_game_allocator_table]] and
  [[g_main_heap_smd_allocator_table]] hold one tag per cell (0 is free);
  the battle heap is a free list of [[battle_heap_node_t]] blocks from
  [[g_battle_heap_rover]], with one list of live blocks per owner.
- Threads: [[native_thread_t]] slots of `0x400` bytes reached through
  [[g_battle_threads]] and [[g_world_threads]]; the running one is
  [[g_battle_thread_current_id]] or [[g_world_thread_current_id]]. Task ids
  ([[native_thread_task_e]]) let threads find each other.
- The game loop: [[g_main_system_game_flow_state]] (2 resets, 3 plays the
  ending), [[g_main_system_go_straight_to_battle]] and
  [[g_main_system_frontend_world_result]] (5 returns to the title).

## Main flow

1. **Boot.** [[main]] runs [[main_boot_run_startup]] (callbacks, GPU, pads,
   SPU and CD reset, the SCEA and Squaresoft logos, memory card and sound
   setup), saves its stack pointer for resets, then enters
   [[main_system_run_game_loop]], which never returns.
2. **The game loop.** Before each title, [[main_boot_reset_game_state]]
   starts a session. Then [[main_overlay_exec_open_bin_main_loop]] runs the
   title, and [[main_overlay_exec_wldcore_and_world_bin]] and
   [[main_overlay_exec_battle_bin]] alternate until a reset or the ending.
   Each loads its overlay from a fixed sector and calls its entry point.
3. **A frame inside an overlay.** The overlay's main loop is thread slot 0.
   Once a frame its event or menu frame function
   ([[battle_script_run_event_frame]], [[world_script_run_frame]]) yields:
   [[battle_thread_yield]] saves its registers and resumes the next running
   slot, each thread runs until its own next yield, and after the last slot
   (15 in BATTLE, 16 in WORLD) control returns to slot 0. Every running
   thread therefore advances once a frame, and [[battle_thread_wait_frames]]
   waits that many frames.
4. **Disc reads while playing.** [[main_file_request_read_bytes]] arms a read
   on the shared descriptor; the overlay's frame loop advances it with
   [[main_file_poll_load]] and tests [[main_file_is_still_loading]]. A thread
   makes such calls through [[battle_thread_call_on_main_stack]], because its
   own stack is about 900 bytes. Waiting loaders
   ([[main_file_load_to_address]], [[main_file_alloc_and_load]]) poll in a
   loop of their own.
5. **Memory.** Files that are uploaded and freed (images, fonts) take game
   heap cells through [[main_heap_alloc]]; menus that open a companion
   overlay first claim the heap's cells with [[main_heap_reserve_at]]; the
   effect stage rebuilds the battle heap ([[battle_heap_init]]) for its own
   blocks.
6. **Soft reset.** [[main_system_reset_game]] stops the display and audio and
   jumps back to the top of the game loop on the stack saved at boot
   ([[main_restore_game_loop_stack_pointer]]).

## Where to change

- **Order of title, world map and battles:** [[main_system_run_game_loop]]
  and its three globals.
- **Boot logos and startup:** [[main_boot_run_startup]],
  [[main_boot_show_sceap_logo]], [[main_boot_fade_in_squaresoft_logo]].
- **Load a file:** [[main_file_alloc_and_load]] for a temporary buffer,
  [[main_file_load_to_address]] for a fixed one, or
  [[main_file_request_read_bytes]] plus polling to keep the frame running.
  Sector numbers are constants in the callers, so moving or growing a file
  waits for the shiftable build.
- **Run a task beside the main loop:** take a slot from
  [[battle_thread_resolve_id]], pass inputs with
  [[battle_thread_set_parameters]], start it with [[battle_thread_start]],
  and yield every frame; stop with [[battle_thread_exit_current]]. WORLD has
  the same functions under `world_thread_`.
- **Memory:** [[main_heap_alloc]] gives 2 KB cells (128 KB in all);
  [[battle_heap_alloc_block]] serves the battle effects.
- **Today's limits:** every change must keep the original bytes; code or data
  that grows or moves waits for the shiftable build (`CODEBASE.md`, "What
  can change today"). The context switch and the main-stack bridge are
  hand-written MIPS, so a port rewrites them.

## Quirks and debts

- [[main_heap_alloc]] can give a block the tag of the block after it, and
  freeing the first then frees both (latent).
- [[battle_heap_alloc_block]] has no out-of-memory exit.
- [[battle_thread_resolve_id]] and its twins return a leftover `$v0` when no
  slot is free, which a main-loop caller receives.
- WORLD runs slot 16, which [[world_thread_reset_scheduler]] does not clear.
- Thread parameters are `s32` but often carry pointers (QUIRKS.md, "Pointers
  held in 32-bit integers").
- [[world_card_run_menu_screen]]'s stack frame reaches into the game heap's
  top cells.

## Functions in scope

![[Engine core scope]]
