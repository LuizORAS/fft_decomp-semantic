---
type: mechanic
tier: 3
scope:
  - battle_menu
  - world_menu
  - world_option
  - world_input
  - world_screen
  - menu_alloc_window_buffer_pair()
  - menu_animate_window_quad_crop()
  - menu_build_quad_pieces()
  - menu_build_window_image_row()
  - menu_confirm_sound_delay_thread()
  - menu_copy_bytes_to_halfwords()
  - menu_copy_ff_terminated_bytes_to_halfwords()
  - menu_copy_unit_data_to_status_billboard()
  - menu_free_high_overlay()
  - menu_get_active_turn_panels()
  - menu_init_panel_fade_tiles()
  - menu_init_sprite_array()
  - menu_lerp_q12()
  - menu_lerp_rect_to_poly_ft4()
  - menu_mini_menu_cancel_delay_thread()
  - menu_request_open_companion_data_file()
  - menu_request_open_companion_executable()
  - menu_request_open_small()
  - menu_reserve_high_overlay()
  - menu_submit_numeric_display_frame_primitives()
  - menu_text_advance_cursor_with_backreferences()
  - menu_tween_window_quads()
  - menu_zoom_cursor_frame()
---

# Menus

## What it does in the game

The battle's menus run on one engine in BATTLE: the turn menu (Move, Act, Wait,
Status, Auto-battle), the skillset and ability lists, the confirmations and
messages, the mini menu (AT, Unit List, Option), the unit panels around them
and the help. WORLD carries a copy of the same engine for its own menus and
screens (the world menu, the Formation screen, the shops), plus a resident copy
of OPTION.OUT's code for the world menu's Options row. Full screens come from
companion overlays loaded on demand: OPTION, BUNIT, JOBSTTS, EQUIP, HELPMENU and
DEBUGCHR ([[Battle Screens]]). The world map's own menus are a separate engine of menu levels in
WLDCORE ([[World Map and Travel]], [[Propositions and the Bar]]).

### Entries and threads

- **A menu is an entry** ([[world_menu_entry_t]]): its window rectangle, text,
  row actions, result cell and thread entry. BATTLE's table is
  `g_battle_menu_idle_action_entries`, indexed by [[battle_menu_id_e]] (the
  turn menus, the lists, the yes/no prompts, the messages).
- **Each open window is a thread.** The menu runs in slot 8, its child in 7 (the
  skillset list under Act) and its grandchild in 6 (the ability list). The
  menu-building slot 3 opens menus and the help, slot 4 runs the system
  functions, and the unit view's panels use slots 10 to 13
  ([[thread_slot_e]]).
- **Rows act** through [[battle_menu_handle_entry_confirm]]: an action from 0 to
  0xff opens that entry in the slot below (bit 0x1000 without the confirm
  sound), a negative action closes that many levels.
  [[battle_menu_handle_entry_cancel]] closes the entry's thread group on Cross.
- **Each frame** [[battle_menu_update_menus_and_event_speed]] runs the event
  script and the menus, then [[battle_menu_resolve_selection]] turns the
  frame's selection into a command for the battle state through
  `g_battle_menu_command_maps`. While a menu thread runs, the game drops to 30
  frames a second (event speed 2).
- **The unit remembers its rows.** The first twelve menus of the command maps
  keep the row each unit last chose (`g_battle_menu_unit_selection_records`), and
  a pending restore reopens the skillset and ability lists the unit had open.

### The turn

1. A battle state opens the action menus
   ([[battle_state_enter_open_action_menus]],
   [[battle_menu_open_active_unit_idle_action_menu]]).
   [[battle_menu_get_id_based_on_mount_moveable_actable]] picks the turn menu
   for what the unit may still do, and [[battle_menu_build_idle_action_menu]]
   places the windows beside the unit and starts
   [[battle_menu_start_entry_thread]].
2. **Act** runs [[battle_menu_run_skillset_thread]] (the skillset list), then
   [[battle_menu_select_unit_action_slots_thread]] (the ability list; Math Skill
   walks three lists), then [[battle_menu_validate_skill_selection_thread]],
   which checks MP, Silence and grayed rows.
3. **Move**, **Wait**, **Status** and **Auto-battle** have their own threads
   ([[battle_menu_confirm_move_command_thread]], [[battle_menu_wait_thread]],
   [[battle_menu_run_status_screen_thread]],
   [[battle_menu_auto_battle_setting_thread]]). The Status screen of a rider or
   a mount is drawn by DEBUGCHR.OUT
   ([[battle_menu_open_rider_status_screen]]).
4. **System functions** are the prompts and messages the battle raises (move
   and target confirmations, ability announcements, crystal or treasure, traps,
   Move-Find items): [[battle_menu_init_system_function]] and
   [[battle_menu_dispatch_system_function]] pick a record of
   `g_battle_system_function_table` ([[battle_menu_system_function_t]]), and
   [[battle_menu_run_system_function_thread]] shows it in slot 4, with the
   record's alternate when navigation messages are off.

### Drawing

- Windows are three buffer pairs ([[menu_alloc_window_buffer_pair]],
  [[battle_menu_draw_active_window_frames]]); work memory comes from an arena
  of 53 pages of 256 bytes ([[battle_menu_alloc_memory]]).
- Text is rendered into 4bpp window images ([[battle_menu_build_window_image]],
  [[battle_menu_display_text]]) and uploaded to VRAM; the five menu texture
  pages are named by [[menu_texture_page_e]].
- Lists and cursors: [[battle_menu_run_icon_selection_loop]] (icon lists with
  a wrapping cursor), [[battle_menu_run_scrolling_ability_list_thread]] (the
  three-column ability list), [[battle_menu_get_cursor_bob_offset]] and
  [[menu_zoom_cursor_frame]].

### WORLD

- WORLD's copy keeps the BATTLE names (`world_menu_*` twins). Its copy of the
  battle menus never runs (QUIRKS.md); the rest drives the world menu
  (`g_world_menu_default_entries`: Options, Brave Story, the tutorials, the
  debug lists) and the full-screen menus.
- **Display scripts.** WORLD's menus draw through a small bytecode
  ([[world_menu_run_display_script]]): a command is an opcode, its length and
  its operands ([[menu_script_opcode_e]]), and 0x1c ends a stream. BUNIT, EQUIP,
  JOBSTTS and CARD use the same opcode numbers with their own handlers, named
  `<overlay>_menu_script_<verb>` like WORLD's ([[Battle Screens]]).
- **Scrollable lists and saved positions:**
  [[world_menu_script_draw_scrollable_list]], [[world_menu_set_selection_record]].
- **Options:** the world menu's Options row
  ([[world_option_open_game_options]]) runs WORLD's resident copy of OPTION.OUT
  ([[world_option_run_menu]]); OPTION.OUT itself is linked at 0x801bf000,
  inside WORLD.BIN.

## Data and structures

| Record | What it holds |
|---|---|
| [[world_menu_entry_t]] | One menu: window, text image, text id, row count, cancel count, row actions, thread entry, result cell, selected row |
| `g_battle_menu_idle_action_entries` ([[battle_menu_id_e]]) | BATTLE's menu entries; `g_menu_thread_menu_data` points at the table in use |
| `g_battle_menu_command_maps` ([[battle_menu_id_record_t]]) | Per menu id, the map from a row to the command the battle state reads |
| `g_battle_menu_unit_selection_records` ([[battle_menu_unit_selection_record_t]]) | Each unit's remembered rows |
| `g_battle_system_function_table` ([[battle_menu_system_function_t]]) | The prompts and messages: menu id, text, entry, row action, cancel count, thread, alternate |
| [[battle_companion_overlay_e]] | The companion overlays the menus load |
| [[menu_script_opcode_e]] | The display script commands of WORLD's menus and the battle screens |

## Main flow

1. **Open.** The battle state builds the turn menu
   ([[battle_menu_build_idle_action_menu]]); [[battle_menu_start_entry_thread]]
   starts the entry's thread in slot 8.
2. **Each frame.** [[battle_menu_update_menus_and_event_speed]] runs the
   threads; rows confirm or cancel ([[battle_menu_handle_entry_confirm]],
   [[battle_menu_handle_entry_cancel]]) and child threads open below.
3. **Resolve.** [[battle_menu_resolve_selection]] maps the result to a
   command, option and item, and the battle state acts on it.

## Where to change

| To change | Edit |
|---|---|
| A menu's rows, text or window | Its entry in `g_battle_menu_idle_action_entries` (BATTLE.BIN data) and its text in the menu text section |
| What a row opens | The entry's row actions |
| What a choice does in battle | `g_battle_menu_command_maps` and the state that reads the command |
| A prompt or message | Its record in `g_battle_system_function_table` |
| An ability list's checks | [[battle_menu_validate_skill_selection_thread]], [[battle_menu_check_action_restrictions]] |
| A WORLD menu's look | Its display script ([[menu_script_opcode_e]]) |
| BATTLE and WORLD alike | Both twins: WORLD's copy is separate code |

## Quirks and debts

- With navigation messages off, [[battle_menu_check_action_restrictions]]
  drops the Charging and Performing messages by comparing the message's high
  byte with the option bits, which only works because both are 0x8000.
- [[menu_alloc_window_buffer_pair]] initializes only the first record
  of the pair, and [[battle_menu_build_window_image]] returns a leftover `$v0`
  on its upload path.
- [[battle_menu_get_cursor_bob_offset]] reads an unset pointer for a mode other
  than 0 or 1.
- WORLD's copy of the battle menus is dead code; `g_battle_menu_cursor_confirm_hold`
  is never set in BATTLE.

## Functions in scope

![[Menus scope]]
