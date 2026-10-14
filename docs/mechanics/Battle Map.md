---
type: mechanic
tier: 3
scope:
  - battle_map
  - battle_weather
---

# Battle Map

## What it does in the game

Every battle and every story scene in BATTLE.BIN plays on a 3D map. A map is
a GNS file (`MAPnnn.GNS`, one per map id, [[map_id_e]]) that lists the files
of the map's resources, each with the map state it is for. BATTLE loads the
resources the current state picks, draws the mesh every frame, lights and
tints it, animates parts of it and runs the weather over it.

| What the player sees | What does it | Code |
|---|---|---|
| The map itself | A primary mesh of textured and untextured triangles and quads, plus up to eight parts with their own animation | [[battle_map_append_mesh_geometry]], [[battle_map_draw_mesh_and_weather]] |
| Walls that vanish when the camera turns | Each textured polygon carries hide bits for eight yaw bands and four resting diagonals, which the queue routines test | `MAP_POLYGON_FLAG_*`, [[battle_map_polygon_flag_command]] |
| A different map by day, by night, in rain or after an event | The GNS rows are picked by the map state: a script variable's value (the layout), the weather and the time of day | [[battle_map_load_data]], [[battle_map_is_gns_record_matching_state]] |
| The sky and the light | A background gradient behind the map, the light colour and direction matrices and the ambient (GTE back) colour, all from the map's lighting resource; events and effects fade them | [[battle_map_dispatch_gns_resource]], [[battle_map_light_state_command]], [[battle_map_update_animations]] |
| Flowing water, flickering torches | 32 texture animations per map: frames moved through VRAM, palette frames, or a rotation of colours | [[battle_map_update_texture_animations]], [[battle_map_start_texture_animation]] |
| Rain, snow and lightning | 32 rain drops with splashes or 64 snowflakes, random lightning in heavy rain | [[battle_map_run_weather_command]], [[battle_map_init_weather_particles]] |
| The coloured tiles of a move range, an ability's range and its targets | The polygons of marked tiles switch to the marked-tile CLUT row and take one colour per kind of mark | [[battle_map_draw_mesh_and_weather]], [[battle_target_tint_marked_tiles]] |
| A map that changes during an event | Event instructions start a part's animation (Use3DObject) or a texture animation, reload the map for a new state, change the weather or tint the whole map | [[battle_script_process_pending_requests]], [[battle_map_color_field]] |
| The Deep Dungeon floors | The map reloads when the number of crystals on the field changes | [[battle_map_check_deep_dungeon_crystal_and_init_state]] |

The tiles a unit stands on (height, slope, surface, depth, the Move-Find
items) belong to the same map but are covered by the [[Movement]] page.

## Data and structures

| Record | What it holds |
|---|---|
| `g_battle_map_gns_lba` | The disc sector of each map's GNS file, 0 for a map id without one |
| [[gns_file_record_t]], [[gns_command_record_prefix_t]] | The GNS rows: a resource file (type, map state, comparison, sector, length), then command rows (indoor, outdoor, snow, camera position) |
| `g_battle_map_gns_records`, `g_battle_map_pending_gns_records`, `g_battle_map_loaded_gns_records` | The first 150 rows, the rows picked for the current state and the rows loaded, by resource type ([[gns_resource_type_e]]) |
| [[map_mesh_file_header_t]] | A mesh file's table of chunk offsets: geometry, palettes, lighting, terrain, animations, part geometry, polygon flags |
| `g_battle_map_mesh_parts` ([[battle_map_mesh_part_t]]) | The primary mesh (part 0) and the eight animated parts: their polygons, matrix, parent and running keyframe |
| `g_battle_map_mesh_animation_keyframes`, `g_battle_map_mesh_animation_instructions` | The parts' keyframes ([[battle_map_mesh_keyframe_t]]) and their eight animation states of instructions each |
| `g_battle_map_textured_triangle_positions` and siblings | The polygon positions, normals, flags and terrain tiles; the GPU packets live in the render buffers |
| `g_battle_map_palette_state` ([[map_palette_state_t]]) | The 14 CLUT rows ([[map_clut_row_e]]) under y 0x1e0: map, marked tiles, sprites, units, items, effects; each CLUT's blend |
| `g_battle_map_texture_animations` ([[battle_map_texture_animation_t]]) | The map's 32 texture animations; `g_battle_map_texture_animation_states` holds the eight colour rotations |
| `g_battle_map_background_gradient_transition`, `g_battle_map_ambient_light_transition`, `g_battle_map_darkness_transition` | The three colour fades: the background gradient, the ambient colour and the darkness colour, the colour of the untextured polygons |
| `g_battle_map_lightning_state` ([[battle_lightning_state_t]]) | The flash timer and the colours it starts from |
| `g_battle_map_weather_particles` ([[battle_weather_particle_t]]) | The snowflakes, or the rain drops (head and tail) and their splashes |

## Main flow

1. **Loading.** A battle's setup runs
   [[battle_map_step_setup_sequence]] under Now Loading, and a map entered
   later runs [[battle_map_step_init_sequence]] behind a black screen. Both
   reset the camera and the palettes, run the loader, load the ENTD units and
   their graphics, and end by seeding the colour fades from the loaded map.
   The setup also rolls the weather for a random battle
   ([[battle_weather_randomize_from_calendar]]) and waits for the deployment.
2. **The loader** ([[battle_map_load_data]]), one step a call: read the GNS,
   pick the rows for the map state (primary mesh, override mesh, texture,
   then every other row whose comparison holds), read each picked file into a
   0x20000-byte buffer and hand it to [[battle_map_dispatch_gns_resource]],
   fill the GPU packets of the mesh parts in both render buffers, then apply
   the command rows ([[battle_map_run_gns_command_record]]) and the event's
   weather.
3. **Each frame** [[battle_state_run_game_loop]] steps the colour fades and
   CLUT blends ([[battle_map_update_animations]]), uploads changed CLUT rows
   ([[battle_map_upload_clut_rows]]) and draws the map
   ([[battle_map_draw_mesh_and_weather]]): texture and part animations,
   lightning, the background gradient, the primary mesh, each part under its
   parent's matrix, the weather particles and the tile marks.
4. **Events** ask for map changes through request globals that
   [[battle_script_process_pending_requests]] carries out on the main loop:
   part animations, texture animations, a mesh refresh
   ([[battle_map_load_mesh_variant]]), a reload for a new map state, a held
   load that a destruction releases, and the weather.

### Weather

The event sets script variable 0x23 (instruction Weather): 0-1 none, 2 light,
3 normal and 4 heavy. The map's command rows say whether it is indoors and
whether it snows there. [[battle_map_run_weather_command]] turns the strength
into particles: rain, with the rain sound from normal strength and random
lightning when heavy, or snow on a snowy map; it does not test the indoor
flag. The rules read the same variable through
[[battle_map_get_effective_weather]], which adds 3 on a snowy map and gives
none indoors.

Before a random battle the weather moves at most one step from its current
value, with odds by date: up most often from November 23 to February 18 and
from May 21 to July 22, down most often from February 19 to May 20 and from
July 23 to November 22, the zodiac's cusps. The time of day is night one
battle in three ([[battle_weather_randomize_from_calendar]]).

### Colours

Three colour records light a map, each with a fade that the event script and
the effect timelines drive ([[map_color_mode_e]] says what a fade starts from):

- **The background gradient**: two colours on a quad behind the map.
- **The ambient colour**: the GTE back colour, added to the light of every lit
  polygon ([[battle_map_control_gte_background_color]]).
- **The darkness colour**: the colour of the untextured polygons, near black
  by default. The battle setup sets it from ATTACK.OUT's first bytes
  ([[attack_sound_resync_scenario_music_and_apply_map_darkness]]).

The CLUT rows fade on their own: [[battle_map_modify_palette]] tints a row or
one CLUT toward a colour (event ColorField tints the whole map row), and the
units' palettes use the same machinery.

## Where to change

| To change | Edit |
|---|---|
| The rain and snow speeds and amounts | [[battle_map_run_weather_command]] (speeds by strength), [[battle_map_init_weather_particles]] (64 flakes, 32 drops) |
| How often lightning strikes | [[battle_map_draw_mesh_and_weather]]: one chance in 480 a frame |
| Random battle weather and time of day | [[battle_weather_randomize_from_calendar]]: the date cutoffs and the drift odds |
| The colours of marked tiles | [[battle_map_draw_mesh_and_weather]]: the move, range and target colour triples |
| Which files a map state loads | The GNS rows (data); the selection passes of [[battle_map_load_data]] and [[battle_map_is_gns_record_matching_state]] |
| A map part's motion | Its keyframes and instructions (data); the tween curves in [[battle_map_update_mesh_part_animations]] |
| Texture animation playback | [[battle_map_update_texture_animations]] (the modes of [[map_texture_animation_mode_e]]) |
| How a colour fade is computed | [[battle_map_blend_ambient_light_color]] and its gradient and darkness twins; the per-frame step in [[battle_map_update_animations]] |

## Quirks and debts

- [[battle_map_load_data]]: the fourth selection pass takes filler rows, and
  the texture quarters load one quarter above where they are uploaded from;
  no retail GNS has quarter rows.
- A map with no GNS sector gets no return value from
  [[battle_map_load_gns_and_move_find_items]].
- GNS command rows apply in file order, so the last of a kind wins; types
  0x87 and 0x8a have no case ([[battle_map_run_gns_command_record]]).
- [[battle_map_load_mesh_variant]] scales the palette offset by four.
- [[battle_map_stop_texture_animation]] stops the slot after the handle's own.
- [[battle_map_light_state_command]] and
  [[battle_map_light_matrix_command]] return an unset pointer for most
  commands; [[battle_map_control_gte_background_color]] too.
- The blend functions leave the colour unset for modes 11 and up.
- An off-map point reads the null tile in [[battle_map_calculate_slope_height]].
- When weather mode 0x96 (effect E458) ends mid-draw over rain, the ordering
  table gets a cycle ([[battle_map_draw_mesh_and_weather]]).
- The map background colour routines (`battle_map_*_background_color`) are
  never called.

## Functions in scope

![[Battle Map scope]]
