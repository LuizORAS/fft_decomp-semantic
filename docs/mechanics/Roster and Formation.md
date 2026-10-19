---
type: mechanic
tier: 2
scope:
  - world_formation
  - world_input_update_lockout_timer()
  - world_input_get_lockout_timer()
  - world_input_filter_menu()
  - world_menu_toggle_unit_status_thread()
  - world_menu_toggle_comparison_status_thread()
  - world_menu_toggle_change_banner_panel_thread()
  - world_menu_toggle_stat_preview_panel_thread()
  - world_menu_toggle_numeric_editor_thread()
  - world_menu_toggle_preview_stats_window()
  - world_menu_toggle_ability_panel_thread()
  - world_menu_start_equipment_and_ability_panel_thread()
  - world_menu_request_unit_panel_redraw()
  - world_menu_get_entry_item_count()
  - world_menu_get_entry_item_equipped_count()
  - world_menu_get_entry_item_icon_source()
  - world_menu_get_selected_unit_hp_bonus()
  - world_menu_get_selected_unit_mp_bonus()
  - world_menu_get_selected_unit_hp_bonus_or_dashes()
  - world_menu_get_selected_unit_mp_bonus_or_dashes()
  - world_menu_draw_item_category_tabs()
  - world_menu_get_learn_job_jp()
  - world_menu_build_selected_ability_prompt()
  - world_menu_is_learn_ability_learned()
  - world_menu_text_entry_wait_thread_for_selected_unit()
  - world_menu_load_sort_key_list()
  - world_menu_store_sort_key_list()
  - world_formation_coords_fit_in_byte()
---

# Roster and Formation

## What it does in the game

The Formation screen is where the player looks after the roster between
battles: equipment, abilities, jobs, dismissals and the order of the units.
The shops reuse its unit grid, records and panels for their own unit screens
(the [[Items and Shops]] page covers those).

- **Getting there.** The world map menu's Formation row starts it
  ([[world_formation_request_open]], then [[world_formation_update_open_request]]
  fades the map out and runs [[world_formation_run_screen]]). An event can ask
  for it before it continues (`EVENT_SCRIPT_VAR_FORMATION_RETURN_EVENT`,
  [[world_formation_run_before_event]]), and WLDCORE's tutorial list runs it
  with one of the ten formation tutorials ([[world_formation_run_tutorial_screen]]).
- **The unit grid.** The roster's 20 records (16 slots and 4 guest slots) are
  shown four to a row, three rows at a time; the grid scrolls when the
  selection leaves them ([[world_formation_update_and_draw_unit_grid]]).
  - Each card shows the unit's sprite (or its egg) and a readout that L2/R2
    cycle: HP, MP, CT, Level/EXP, Brave/Faith ([[world_formation_draw_unit_readout]]).
    HP and MP are always shown full; CT shows dashes outside battle.
  - Circle opens the selected unit's status (Circle again opens View
    Abilities, X closes it); Square turns the unit banner on and off. While a
    unit or a menu is open, L1/R1 step through the units, skipping eggs.
  - X on the grid fades the screen and the music out and leaves; the records
    are saved back to the roster ([[world_formation_save_records_to_party_data]]).
- **The unit menu.** Triangle or Start opens "Item / Ability / Change Job /
  Remove Unit / Order Unit" ([[world_formation_run_unit_grid_menu]]); for an
  egg it asks "Destroy the monster egg?" instead. A unit away on a
  proposition, a monster, an egg or a Mime is locked out of some rows
  ([[world_formation_get_unit_lock]]).

  | Row | Menus | Notes |
  |---|---|---|
  | Item | Equip / Best / Remove / List | Equip takes items from the inventory, previewing the stats; Best equips the best fit after "O K / N O"; Remove takes one item or (Left) all of them off; List is the inventory by category ([[world_formation_run_item_menu]]) |
  | Ability | Set / Remove / Learn | Set fills the secondary skillset and the reaction, support and movement slots; Remove empties one or (Left) all; Learn spends JP in a chosen job's list. A Mime may only Learn ([[world_formation_run_ability_menu]]) |
  | Change Job | the job wheel | Left/Right spin the wheel of jobs (those the unit cannot take are dimmed); Circle plays the 240-frame job-change animation (Circle or X skips ahead) and changes the job; a star marks a job whose abilities are all learned ([[world_formation_run_job_wheel_menu]]) |
  | Remove Unit | yes/no | Ramza and the guest slots cannot be removed, and a unit on a proposition is "on assignment"; story characters answer with their own lines ([[world_formation_run_dismiss_unit_menu]]) |
  | Order Unit | seven sort keys | Choosing a key moves it to the front of the priority list and re-sorts the roster ([[world_formation_run_order_unit_menu]]) |

- **Equipment rules.** A job equips the types its data allows
  ([[world_unit_can_equip_item_id]]). The two hands pair as follows
  ([[world_formation_can_equip_item_in_slot]]):
  - two weapons need Two Swords and two Two Swords compatible weapons;
  - two shields never pair, and a shield never pairs with a two-handed-only
    weapon.

  Equipping an item that does not pair sends the other hand's item back to
  the inventory ([[world_formation_equip_item_to_unit_slot]]).
- **Job changes.** After a job or support change the unit drops what it can
  no longer use ([[world_formation_remove_invalid_unit_loadout]]):
  - items its new job cannot equip, and a second weapon without Two Swords;
  - every ability for a Mime;
  - a support its job already has innately (Chemist's Throw Item, Monk's
    Martial Arts, Mediator's Monster Talk, Ninja's Two Swords);
  - a secondary skillset equal to the primary.

  With the Max Equip at Job Change option on, a job change, or an Equip,
  Martial Arts, Two Hands or Two Swords support coming or going, re-equips
  the best fit and says "Equip to the maximum"
  ([[world_formation_change_unit_job]], [[world_formation_set_unit_ability_slot]]).
- **The order.** The roster is sorted by a priority list of seven keys,
  kept in the save: Number (the roster slot, ascending), Level, Job, Brave,
  Faith, MAX HP and MAX MP (descending). A tie falls to the next key, and
  eggs always go last ([[world_formation_sort_roster]]).

## Data and structures

| Record | What it holds |
|---|---|
| [[world_unit_t]] | A formation record (0x128 bytes): the display stats, identity, job, equipment, abilities, job levels and JP of a roster unit, built by [[world_formation_build_unit_record]] |
| [[g_world_formation_unit_records]], [[g_world_formation_unit_pointers]], [[g_world_formation_record_order]] | The records, the sorted pointers the screens index, and the sort order; record 20 is the preview record (`WORLD_FORMATION_PREVIEW_RECORD`) the menus try equipment on |
| [[g_world_formation_menu_handlers]] | The screen's 25 states ([[world_formation_menu_e]], WORLD `0x8018baec`), one handler each |
| [[g_world_sort_key_lists]], [[g_main_item_type_order_1]], [[g_world_formation_sort_field_indices]] | The Order Unit keys (saved in MAIN) and the record field each key compares |
| [[g_world_formation_dismiss_entries]] | Per story character's sprite set, the script variable a dismissal sets to `0xC` |
| [[world_formation_unit_lock_e]] | Why a unit cannot use a menu: proposition, monster, egg, Mime |

The menu texts are WORLD's resident formation text (section 27, ids
`0xd800 +` n: the menu rows, "Can't change", "Equip to the maximum") and
formation messages (section 24, `0xc000 +` n: the refusals and the
dismissal prompt).

## Main flow

1. **Start.** [[world_formation_run_screen]] binds the packet buffers, resets
   the screen ([[world_formation_init_menu_state]]: the menu display, then
   the records through [[world_formation_build_record_list]]) and starts the
   formation music or the tutorial.
2. **Each frame.** It runs the help and banner updates, the current state's
   handler from [[g_world_formation_menu_handlers]], the unit grid or its
   background, the L1/R1 buttons, the selected unit's panels
   ([[world_formation_update_unit_selection]]) and the menu sounds.
3. **The handlers.** The grid and its unit menu
   ([[world_formation_run_unit_grid_menu]]), Item
   ([[world_formation_run_item_menu]]), Ability
   ([[world_formation_run_ability_menu]]), View Abilities
   ([[world_formation_run_view_abilities_menu]]) and the job wheel
   ([[world_formation_run_job_wheel_menu]]). Item, Ability and the wheel slide
   the grid out and back ([[world_formation_step_grid_transition]],
   [[world_formation_step_grid_return_transition]]).
4. **Edits.** Equipment, ability and job changes go through
   [[world_formation_equip_item_to_unit_slot]],
   [[world_formation_set_unit_ability_slot]] and
   [[world_formation_change_unit_job]], which save the roster each time.
5. **Exit.** The screen waits for its threads, saves the records to the
   roster and resets the menu context.

## Where to change

- **Who can be removed:** [[world_formation_can_dismiss_unit]]; the story
  variables a dismissal sets: [[g_world_formation_dismiss_entries]].
- **Who is locked out of the menus:** [[world_formation_get_unit_lock]].
- **Hand pairing:** [[world_formation_can_equip_item_in_slot]] (and EQUIP's
  [[equip_unit_can_equip_item_id]] in BATTLE).
- **What a job change keeps:** [[world_formation_remove_invalid_unit_loadout]];
  the supports that trigger a re-equip: [[world_ability_is_equip_support_change]].
- **The order:** [[g_world_formation_sort_field_indices]] (which field a key
  compares) and [[world_formation_sort_roster]] (direction, eggs last).
- **The readouts on the cards:** [[world_formation_draw_unit_readout]] and the
  card callbacks set by [[world_formation_init_selection_cursor]].

[[Changing the formation and roster]] walks through these changes.

## Quirks and debts

- [[world_formation_rebuild_unit_list]]: the soldier office stages unit 0 of
  an empty list, copying kernel RAM.
- Formation states 22 and 23 have handlers and no code stores them, so
  [[world_formation_step_unit_portrait_slide]] never runs.
- [[world_formation_get_unit_palette_index]] reads an unset `job` and past a
  table for story identities that never join.
- [[world_formation_get_unit_stat_by_mode]] and
  [[world_formation_run_set_ability_menu]] read locals that some paths leave
  unset.
- [[world_formation_show_unit_message]] is referenced nowhere.
- The formation unloads its music without clearing the playing record
  ([[main_sound_unload_scenario_mus]]).

## Functions in scope

![[Roster and Formation scope]]
