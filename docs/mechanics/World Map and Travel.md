---
type: mechanic
tier: 3
scope:
  - wldcore_map
  - wldcore_calendar
  - wldcore_system
  - wldcore_file
  - wldcore_input
  - wldcore_gfx_start_screen_fade()
  - wldcore_gfx_step_screen_fade()
  - wldcore_gfx_draw_screen_fade_overlays()
  - wldcore_entrypoint()
  - wldcore_system_run_requested_formation()
  - wldcore_system_finalize_loop()
  - wldcore_file_wait_for_load()
  - wldcore_gfx_clear_screen_and_set_map_clip_rect()
  - wldcore_gfx_init_subsystem()
  - wldcore_gfx_capture_map_backdrop()
  - wldcore_gfx_update_map_backdrop()
  - wldcore_gfx_draw_world_frame_with_map()
  - wldcore_gfx_draw_world_frame()
  - wldcore_gfx_toggle_captured_world_frame()
  - wldcore_gfx_set_display_rect()
  - wldcore_gfx_clear_vram_and_scratch()
  - wldcore_gfx_copy_color_with_tint()
  - wldcore_gfx_draw_projected_map_tiles()
  - wldcore_gfx_draw_calendar_date()
  - wldcore_gfx_draw_transition_screen()
  - wldcore_menu_push_root_level()
  - wldcore_menu_resume_root_level()
  - wldcore_menu_suspend_root_level()
  - wldcore_menu_reset_to_root_level()
  - wldcore_menu_step_map_cursor_level()
  - wldcore_menu_push_focus_location_level()
  - wldcore_menu_step_focus_location_level()
  - wldcore_menu_push_map_path_level()
  - wldcore_menu_step_map_path_level()
  - wldcore_menu_push_map_location_visibility_level()
  - wldcore_menu_step_map_location_visibility_level()
  - wldcore_menu_dispatch_pending_script_requests()
  - wldcore_menu_restore_level_on_world_entry()
  - wldcore_menu_push_location_menu_level()
  - wldcore_menu_step_location_menu_level()
  - wldcore_menu_run_shop_and_rebuild_screen()
  - wldcore_menu_push_event_transition_level()
  - wldcore_menu_step_event_transition_level()
  - wldcore_menu_push_world_menu_level()
  - wldcore_menu_step_world_menu_level()
  - world_map_destination_list_thread()
  - world_map_confirm_destination()
---

# World Map and Travel

## What it does in the game

Between battles the party stands on the world map, a 2D map seen through a
camera that can pan and zoom. WLDCORE runs it, with WORLD loaded above it
for the menus and shops.

- **Locations.** 43 locations: 15 towns, the castles and other places, and
  19 fields (Mandalia Plains to Germinas Peak). A location's dot shows once
  its visibility variable is set, and its colour tells the player what waits
  there:
  - **red:** a story event;
  - **blue:** a town or another place with nothing pending;
  - **green:** a field with nothing pending.
- **Routes.** 48 routes join pairs of locations. Each is drawn once its
  variable is set; story scripts draw and erase them with an animation.
- **The cursor.** The D-pad moves a cursor that speeds up and is pulled
  onto the nearest dot; the map scrolls when the cursor nears the edge. The
  other buttons:

  | Button | Action |
  |---|---|
  | L1, R1 | Zoom out to the whole map and back |
  | Square, held | Scroll the map freely, with arrows at the edges |
  | Select, on a dot | That location's help text |
  | Triangle, Start | WORLD's menu: the formation, the location list, Brave Story, the tutorials, the memory card |

  The name of the location under the cursor shows beside its dot.
- **Travelling.** Circle on another dot walks the marker there by the path
  with the fewest legs, the cheapest on a tie, along visible routes only
  ([[wldcore_map_find_best_route_path]]). WORLD's location list can also pick
  the destination. At every location the marker reaches, in order:
  - a day passes;
  - a red location's story event stops the walk;
  - a field may start a random battle, rolled for the road just taken;
  - otherwise the walk goes on.

  At the destination, a red location offers its event choices.
- **Locations' menus.** Circle on the current location opens its menu, if it
  has one:
  - a town: Bar, Shop and Soldier Office, plus the Fur Shop at Dorter,
    Warjilis and Zarghidas once it is enabled;
  - the Deep Dungeon: its floors, each a battle;
  - a red location: its story choices.
- **Random battles.** Each field lists up to three roads into it. The roll is
  made on arriving at the field ([[wldcore_map_roll_random_encounter]]):
  - **Tier:** the first of the road's four tiers whose threshold is at or
    above the road's progress variable.
  - **Chance:** a roll of 0 to 99 at or below the tier's chance starts a
    battle, so a chance of 30 is 31 in 100.
  - **Enemy squad:** one of the ENTDs that the tier's mask enables, drawn
    evenly.

  In the retail table every road has chance 30 and reads the shop level
  (`EVENT_SCRIPT_VAR_SHOP_ITEM_AVAILABILITY`). The first tiers (thresholds
  3 or 4 on the early roads, then 12 or 14) enable two to four ENTDs; the
  tiers of 16 and 25 enable all eight. The third road of 13 fields is unused
  (route 0, chance 0).
- **Calendar.** The date advances one day per location reached, and by
  whole spans when a story event sets a date advance. Each day:
  - the propositions count down;
  - Brave Story characters age on their birthdays;
  - monster eggs count down to hatching
    ([[world_unit_update_monster_breeding]]).

  A year count rises on March 21, the first day of Aries. The map shows the
  date as the month's label (the game names its twelve months after the
  zodiac signs) and the day of that calendar month, not the day within the
  sign ([[wldcore_gfx_draw_calendar_date]]).
- **Leaving the map.** A story event or a random battle leaves through a
  transition:
  - a battle spins the screen away, then flashes;
  - a story event spins it in and whites out;
  - a scene started from a menu fades.

  Then [[wldcore_entrypoint]] returns the world result to MAIN.

## Data and structures

| Record | What it holds |
|---|---|
| [[g_wldcore_map_dots]] ([[wldcore_map_dot_t]]) | The 43 location dots: kind, map position, display flags, palette, screen position; built by [[wldcore_map_color_and_draw_dots]] from `g_wldcore_map_dot_sources` |
| [[g_wldcore_map_projection_state]] ([[wldcore_projection_state_t]]) | The camera's direction flags and angle, and the player's marker as a dot (its kind is the current location) |
| [[g_wldcore_map_route_tables]], [[g_wldcore_map_route_polylines]] | The 48 routes: their header ([[wldcore_route_record_t]]: point count, the two locations, the cost) with the points the marker follows, and the ribbon drawn on the map |
| [[g_wldcore_map_travel]] ([[wldcore_map_travel_t]]) | The path found and the leg, segment and frame the marker is on |
| `g_wldcore_random_battle_data` | Per field, three roads: route, deployment squad, four tiers (threshold, chance, ENTD mask), eight ENTDs, battle map, progress variable |
| [[g_wldcore_location_records]] ([[wldcore_state_record_t]]) | Per location: its picture and whether it is a town |
| [[g_wldcore_days_per_month]], [[g_wldcore_zodiac_start_dates]], [[g_wldcore_brave_story_birthdays]] | The calendar's tables: month lengths (no leap years), the first day of each sign, the Brave Story birthdays |
| [[g_wldcore_map_label_location]] | The location whose name label is drawn |
| Script variables | `EVENT_SCRIPT_VAR_LOCATION` (where the marker is), `_LOCATION_VISIBLE_FIRST`, `_PATH_VISIBLE_FIRST`, `_LOCATION_COLOR_FIRST` (0 is red), `_LOCATION_CHOICE_HIDDEN_FIRST`, `_MONTH`, `_DAY`, `_DATE_ADVANCE`, `_YEAR_COUNT` |

The menu levels are records of the WLDCORE menu stack ([[wldcore_menu_level_type_e]]):

| Level | Push / step | What it does |
|---|---|---|
| `ROOT` | [[wldcore_menu_push_root_level]] / [[wldcore_menu_step_map_cursor_level]] | The map cursor |
| `WORLD_MENU` | [[wldcore_menu_push_world_menu_level]] / [[wldcore_menu_step_world_menu_level]] | WORLD's menu over the map, and what it asks for when it closes |
| `LOCATION_MENU` | [[wldcore_menu_push_location_menu_level]] / [[wldcore_menu_step_location_menu_level]] | A location's menu |
| `EVENT_TRANSITION` | [[wldcore_menu_push_event_transition_level]] / [[wldcore_menu_step_event_transition_level]] | Leaving for a battle or a story event |
| `FOCUS_LOCATION` | [[wldcore_menu_push_focus_location_level]] / [[wldcore_menu_step_focus_location_level]] | The camera pans to a location |
| `MAP_PATH` | [[wldcore_menu_push_map_path_level]] / [[wldcore_menu_step_map_path_level]] | A route is drawn or erased |
| `MAP_LOCATION_VISIBILITY` | [[wldcore_menu_push_map_location_visibility_level]] / [[wldcore_menu_step_map_location_visibility_level]] | A location appears or disappears |

## Main flow

1. **Entry.** MAIN loads WLDCORE (and WORLD) and calls
   [[wldcore_entrypoint]].
2. **A visit.** First, [[wldcore_system_run_requested_formation]] runs a formation that
   an event asked for. Then [[wldcore_system_init_subsystems]] sets the visit up:
   - the state and the map texture;
   - the GPU and the tile grid;
   - WORLD's menus and a pending date advance;
   - the dots, the marker and the camera;
   - the root level.

   [[wldcore_menu_restore_level_on_world_entry]] then opens the location's
   arrival text, its map requests, or the screen a title or replay asked
   for.
3. **Each frame**, while `MAIN_SYSTEM_FLAG_RUNNING` is set:
   - the fade advances;
   - the dots pulse;
   - the top menu level's step runs.

   The map redraws only on frames when something moves on it
   (`MAIN_SYSTEM_FLAG_DRAW_MAP`, [[wldcore_gfx_draw_world_frame_with_map]],
   which also steps the travel). Other frames draw over a still of the map
   ([[wldcore_gfx_draw_world_frame]]), captured when the map stops moving
   ([[wldcore_gfx_update_map_backdrop]]).
4. **Travel.** [[wldcore_map_start_travel]] finds the path, and
   [[wldcore_map_step_route_travel]] walks the marker. At each location
   reached, it checks:
   - [[wldcore_calendar_advance_day]];
   - the location's story event;
   - [[wldcore_map_roll_random_encounter]].
5. **Leaving.** A story event, a random battle or a Deep Dungeon floor pushes
   the event transition. Its last step clears `MAIN_SYSTEM_FLAG_RUNNING`.
   [[wldcore_system_finalize_loop]] publishes the next map, and the entry point
   returns 0 (story event), 1 (random battle), 2 (tutorial or replay),
   4 (Deep Dungeon) or 5 (title). A loaded game
   (`MAIN_SYSTEM_FLAG_RESTART`) starts a new visit instead.

## Where to change

- **The random battle chance, tiers and enemies:** the encounter table that
  [[wldcore_map_roll_random_encounter]] reads. The road is matched by the
  route the marker took into the field.
- **Which path the marker takes:** the route costs in the route headers, and
  the "fewest legs first" rule in [[wldcore_map_find_best_route_path]].
- **Travel speed:** the segment frame count in
  [[wldcore_map_begin_route_segment]], which is twice the point's distance.
- **Days per location and what a day does:** [[wldcore_map_step_route_travel]]
  (one [[wldcore_calendar_advance_day]] per leg) and
  [[wldcore_calendar_advance_day]].
- **A town's services:** [[wldcore_map_build_location_menu_entries]] and
  [[wldcore_menu_step_location_menu_level]].
- **The map controls:** [[wldcore_menu_step_map_cursor_level]].
- **Where a story waits:** the location scripts (the WLDCORE opcodes) and the
  colour, visibility and route variables.

[[Changing travel and random encounters]] walks through these changes.

## Quirks and debts

- [[wldcore_map_roll_random_encounter]]:
  - the chance test is off by one (31 in 100 for 30);
  - the ENTD masks count from the high bit.
- [[wldcore_calendar_advance_days]]:
  - an event's date advance does not count toward the propositions still
    out;
  - it misses birthdays after December 31;
  - it counts the year at the year end instead of on March 21.
- [[wldcore_map_step_route_travel]] stores a 0-based location in the cursor
  level's one-based hovered location. The cursor level's `pending_mode` is
  never set.
- [[wldcore_menu_step_map_path_level]] ends the path sound with sound id 0.
- [[wldcore_input_check_repeating_directional]] tests and clears its repeat
  counters in a different order from their layout.
- The zodiac start table has a second copy in OPEN.

## Functions in scope

![[World Map and Travel scope]]
