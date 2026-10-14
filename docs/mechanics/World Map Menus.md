---
type: mechanic
tier: 3
scope:
  - wldcore_menu
  - wldcore_list
  - wldcore_window
  - wldcore_text
  - wldcore_gfx_dispatch_display_object_list()
  - wldcore_gfx_draw_animated_display_object()
  - wldcore_gfx_draw_display_object_sprite()
  - wldcore_gfx_calculate_scaled_rectangle()
  - wldcore_gfx_step_dissolve_image_upload()
  - wldcore_gfx_load_picture_set()
  - wldcore_gfx_load_background_set()
  - wldcore_gfx_load_location_picture()
  - wldcore_gfx_draw_context_value_display()
  - wldcore_gfx_draw_cursor_sprite_and_underline()
  - wldcore_gfx_draw_number_digits()
  - wldcore_gfx_draw_number_digits_with_fill()
  - wldcore_text_load_message_block_if_changed()
  - wldcore_gfx_reset_context_value_display()
  - wldcore_unit_start_status_panel_threads()
  - wldcore_unit_build_status_panel_data()
---

# World Map Menus

## What it does in the game

Everything WLDCORE shows over the world map runs on one stack of menu levels:
the map cursor and a location's menu, the Bar, the Brave Story, Professor
Daravon's tutorials, the sound novels and the debug tools. WORLD's own menus
(the world menu, Formation, the shops, the card screen) are a separate engine
([[Menus]]) that WLDCORE runs once a frame as one level of the stack. The map
levels belong to [[World Map and Travel]], the Bar's to
[[Propositions and the Bar]] and the sound novels' script to [[Event Scripts]];
this page covers the stack itself and the screens built on it.

### The menu stack

- **A level** is a type ([[wldcore_menu_level_type_e]]) in
  `g_wldcore_menu_stack_types` and a 0x5c-byte record
  ([[wldcore_menu_stack_record_t]]) in `g_wldcore_menu_stack_records`, at
  `g_wldcore_menu_stack_depth`. A push routine fills the next record, stores
  the type and raises the depth; a level pops itself by lowering it.
- **Three handler tables** in WLDCORE.BIN, indexed by the type:
  - `g_wldcore_menu_idle_handlers`: the level's per-frame step, which
    [[wldcore_entrypoint]] runs through
    [[wldcore_menu_dispatch_idle_handler]];
  - `g_wldcore_menu_suspend_handlers`: run before another level opens over it
    ([[wldcore_menu_dispatch_suspend_handler]]); it dims the level's windows
    and holds its cursor, and a list withholds every button;
  - `g_wldcore_menu_resume_handlers`: run once that level has closed
    ([[wldcore_menu_dispatch_resume_handler]]); it undoes the suspend and
    shows the level's prompt again.
- **Buttons.** WLDCORE reads the pad itself and passes WORLD's menus only the
  buttons not in `g_wldcore_menu_blocked_buttons`
  ([[wldcore_menu_run_world_frame]]). A list built by WORLD's panel thread
  withholds Circle, Cross and Select, so its WLDCORE input step handles them.
- **Messages.** The prompts of Alazlam, Daravon and the Bar run in the message
  box, WORLD thread 14 (`WLDCORE_THREAD_SLOT_MESSAGE`).
  [[wldcore_menu_push_message_level]] shows a help text in WORLD thread 2 as a
  level of its own, and [[wldcore_menu_finish_close_request]] pops it and
  resumes the level underneath. Long texts use the scrollable text window
  (`g_wldcore_scrollable_text_window`, [[wldcore_text_init_scrollable_window]],
  [[wldcore_text_is_window_finished]]).
- **Full screens** open through a screen transition
  ([[wldcore_menu_push_screen_transition_level]]): the map is captured, the
  screen fades, and [[wldcore_menu_step_screen_transition_level]] pushes the
  Brave Story, a tutorial list or a treasure's detail.

### Windows and pictures

- **Window records** ([[wldcore_window_record_t]], 36 bytes) are animated
  sprites: cursors, frames, button prompts. **Render records**
  ([[wldcore_window_render_record_t]], 52 bytes) are images in VRAM: text
  panels built in `g_wldcore_window_image_buffer`
  ([[wldcore_window_build_render_record_image]],
  [[wldcore_window_load_image_record_to_vram]]) and pictures.
- A level appends its records to the display-object queue
  ([[wldcore_window_append_record_and_reset_color]],
  [[wldcore_window_append_render_record_and_reset_color]]) and releases them
  when it closes. [[wldcore_gfx_dispatch_display_object_list]] draws the queue each
  frame, skipping objects flagged hidden or blank.
- `palette` picks a record's CLUT: the suspend handlers give a covered level
  CLUT 10 on its cursor window, 6 on its side windows, 5 on the Brave Story's
  upper and lower windows and 2 on its text.
- Land and treasure pictures come from WLDPIC.BIN
  ([[wldcore_gfx_load_picture_set]]) and the sound novels' backgrounds through
  [[wldcore_gfx_load_background_set]]; a treasure's picture dissolves in
  ([[wldcore_gfx_step_dissolve_image_upload]]).

### The Brave Story

Opened from the world menu, or from the title screen with
`MAIN_SYSTEM_FLAG_TITLE_BRAVE_STORY`. [[wldcore_menu_push_brave_story_level]]
shows the sections that [[wldcore_menu_build_brave_story_entries]] lists
(text 0xb83f + row) and the play time; Alazlam's lines run in the message box.

| Section | Level | What it shows |
|---|---|---|
| Fact | FACTS ([[wldcore_list_open_facts]]) | The facts set (variables 0x1a4-0x1bb), each a text; never listed in retail (QUIRKS.md) |
| Record | STORY_EVENTS ([[wldcore_list_open_story_events]]) | The story events seen, each a text; Triangle replays the event ([[wldcore_menu_handle_story_event_text_input]]) |
| Person | PERSONS ([[wldcore_list_open_persons]]) | The people met (variables 0x3c0-0x3ff), each a profile with a portrait ([[wldcore_menu_push_person_detail_level]]) |
| Job | PROPOSITION_CATEGORY ([[wldcore_menu_push_proposition_category_level]]) | Advancing: the propositions in progress, their units and each unit's status panels ([[wldcore_list_open_active_propositions]]); Work history: the completed ones and their last attempt ([[wldcore_list_open_proposition_history]]) |
| Unexplored Land | UNEXPLORED_LANDS ([[wldcore_list_open_unexplored_lands]]) | Each land found, with its picture and discovery date |
| Treasure | TREASURES ([[wldcore_list_open_treasures]]) | Each treasure found, with its picture and date; a book opens its sound novel |

**Sound novels.** Triangle on a book's detail
([[wldcore_menu_step_treasure_detail_level]]) opens the reader
([[wldcore_menu_push_sound_novel_level]]), or first the prompt to resume a
book left unfinished ([[wldcore_menu_push_sound_novel_resume_level]]: Restart
or Read from start). In the reader, a Quit / Continue prompt
([[wldcore_menu_step_sound_novel_quit_level]]) saves the place, and the
history ([[wldcore_menu_push_sound_novel_history_level]]) shows the sections
read last. The novel's script is in [[Event Scripts]].

**Status panels.** A participant of a proposition in progress
([[wldcore_menu_push_participant_detail_level]]) and a unit in the Bar's
send-unit list show the unit status panels
([[wldcore_unit_build_status_panel_data]]); Select there opens the status help
([[wldcore_menu_push_status_help_level]]), a cursor over the panels' fields
(`g_wldcore_status_help_fields`) that shows each field's help.

### Professor Daravon's tutorials

[[wldcore_list_open_tutorial_categories]] shows Organization, Battle, Tips and
Return (text 0xb8d9 + row):

- **Organization** ([[wldcore_list_open_formation_tutorials]]): the Formation
  screen's seven lessons, run on that screen
  ([[world_formation_run_tutorial_screen]]);
- **Battle** ([[wldcore_list_open_battle_tutorials]]): eight lessons played as
  events, which return to this list;
- **Tips** ([[wldcore_list_open_tutorial_topics]]): 41 texts.

A watched lesson is flagged in `g_main_tutorial_progress_masks`.

### Debug tools

The world menu's debug rows open three debug levels: the
script-variable list ([[wldcore_menu_push_variable_list_level]]) with its
editor ([[wldcore_menu_push_variable_value_panel_level]]), and the Party number
panel ([[wldcore_menu_push_party_number_panel_level]]), which rebuilds the
party from an ENTD.

## Data and structures

| Record | What it holds |
|---|---|
| [[wldcore_menu_level_type_e]] | The level types, which index the three handler tables |
| [[wldcore_menu_stack_record_t]] | One level's record; a union of the views each level uses |
| `g_wldcore_menu_idle_handlers`, `g_wldcore_menu_suspend_handlers`, `g_wldcore_menu_resume_handlers` | The handlers of each level type |
| [[wldcore_window_record_t]], [[wldcore_window_render_record_t]] | Window and render records, in `g_wldcore_window_records` and `g_wldcore_window_render_records` |
| [[wldcore_menu_cursor_state_t]] ([[wldcore_cursor_slot_e]]) | Each list's remembered row and scroll, `g_wldcore_menu_cursor_states` |
| [[wldcore_text_scrollable_window_t]] | The paged text window shared by the detail levels |
| [[wldcore_brave_story_row_e]] | The Brave Story's sections |
| `g_wldcore_story_event_ids`, `g_wldcore_tutorial_event_ids` | The events that the Record and the battle tutorials replay |
| `g_wldcore_unexplored_land_picture_sets`, `g_wldcore_treasure_picture_sets`, `g_wldcore_treasure_sound_novel_ids` | Each land's and treasure's picture, and the novel a treasure opens |

## Main flow

1. **Open.** A world menu row raises a request flag
   (`MAIN_SYSTEM_FLAG_REQUEST_BRAVE_STORY`, `_TUTORIAL`, ...);
   [[wldcore_menu_step_world_menu_level]] pops the world menu level and opens
   the screen through [[wldcore_menu_push_screen_transition_level]].
2. **Each frame.** [[wldcore_menu_dispatch_idle_handler]] steps the top level.
   A list's step reads Circle, Cross and Select: Circle closes the list and
   opens the chosen entry, Cross closes it, and Select suspends it under a help
   message.
3. **Back.** A closing level releases its records, lowers the depth and runs
   [[wldcore_menu_dispatch_resume_handler]] for the level underneath.

## Where to change

| To change | Edit |
|---|---|
| What a level does each frame, before a child opens, or after it closes | Its handlers in the three tables (WLDCORE.BIN data) and the functions they name |
| The Brave Story's sections | [[wldcore_menu_build_brave_story_entries]], [[wldcore_menu_handle_brave_story_input]] |
| What a section lists | Its `wldcore_list_open_*` routine: the script variables it walks and the text base of the names |
| A land's or treasure's picture | `g_wldcore_unexplored_land_picture_sets`, `g_wldcore_treasure_picture_sets` |
| Which treasure opens a sound novel | `g_wldcore_treasure_sound_novel_ids` |
| The tutorial lessons | `g_wldcore_tutorial_event_ids` (battle), WORLD's tutorial scripts (Organization), text 0x8000-0x8028 (Tips) |
| A help message | The text id the list step passes to [[wldcore_menu_push_message_level]] |

## Quirks and debts

- The Fact section never appears: nothing sets the flag that gates it
  (QUIRKS.md).
- Only treasure 31 names a sound novel, so the other book treasures open no
  reader.
- The land and treasure details' first phase waits on a render record that the
  push never set.

## Functions in scope

![[World Map Menus scope]]
