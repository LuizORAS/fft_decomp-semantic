---
type: mechanic
tier: 3
scope:
  - battle_camera
  - world_camera
  - camera_clear_zoom_variable_low_bit()
  - camera_fusion_thread()
  - camera_init_tilt_and_zoom()
  - camera_normalize_yaw_angle()
  - camera_pick_nearest_unblocked_yaw()
  - camera_step_track()
  - camera_store_state_to_script_variables()
  - camera_store_yaw_band()
  - camera_thread()
  - camera_wait_until_idle()
  - camera_wrap_yaw_angle()
---

# Camera

## What it does in the game

BATTLE draws the map, the units and the effects through one camera: three
angles, a position, three zoom scales and a screen offset, rebuilt into the
camera matrix every frame ([[battle_camera_update_matrices]]). Five things
move it:

| Who | What the player sees | Code |
|---|---|---|
| The player | L1 and R1 turn a quarter turn, coming to rest on a diagonal; holding the button keeps turning, faster each frame. L2 zooms between in (scale 1.0) and out (0.75), R2 tilts between low (pitch 0x12e) and high (0x1c0). The d-pad pans in the states that free the cursor | [[battle_camera_handle_rotation_input]], [[battle_camera_handle_zoom_input]], [[battle_camera_handle_tilt_input]], [[battle_camera_update_map_pan]] |
| Following | The camera eases toward the cursor tile, and toward the acting unit while it moves, picks a facing or starts an attack; a unit whose tile is hidden from the current side makes the camera turn | [[battle_camera_update_vector_from_game_state]], [[battle_camera_update_cursor_tile_vector]], [[battle_camera_rotate_when_unit_tile_not_visible]] |
| Events | The CAMERA instruction moves seven components (position, angles, zoom) to targets over a number of frames along a curve; a fusion runs several CAMERA instructions as one smooth path; Focus turns to the nearest diagonal that shows two units | [[camera_thread]], [[camera_fusion_thread]], [[camera_pick_nearest_unblocked_yaw]] |
| Effects | An ability's effect timeline has three camera tracks (angles, position, zoom) whose keyframes aim at the caster, a target, the cursor tile or saved values; effect scripts also save, restore and set the camera | [[battle_camera_update_effect_tracks]], [[battle_camera_execute_angle_command]], [[battle_camera_execute_position_command]], [[battle_camera_execute_zoom_command]] |
| A new map | The map's camera record places the camera and the cursor; the defaults are the low tilt, zoomed in and yaw 0xe00 | [[battle_camera_set_focus_and_cursor_tile]], [[battle_camera_init_defaults]] |

Sprites are drawn in the camera's order too: a unit, its shadow, the cursor
and a status bubble are linked into the ordering table at the depth of one
point near their tile, which the camera computes for the current yaw
([[battle_camera_get_tile_sort_point]], [[battle_camera_get_step_sort_point]]).

WORLD carries a copy of the event camera (`world_camera_*`), but WORLD's
event interpreter never runs, so neither does the copy (see the quirks).

## Data and structures

| Record | What it holds |
|---|---|
| `g_battle_camera_angles` | The angles (an SVECTOR): pitch, yaw and roll, 0x1000 a full turn |
| `g_battle_camera_current_real_coords` | The camera position (20.12) |
| `g_battle_camera_zoom` | The three zoom scales (ONE is 1.0) |
| `g_battle_offset_screen_coords` | The screen offset of the matrix's translation |
| `g_battle_camera_matrix` | The matrix every draw uses |
| `g_battle_camera_step_vector` | This frame's following step |
| [[battle_camera_rotation_action_e]], [[battle_camera_step_action_e]] | The turn, tilt and zoom in progress, toward `g_battle_camera_destination_yaw`, `g_battle_camera_tilt_target` and `g_battle_camera_zoom_target` |
| [[battle_camera_track_t]], [[battle_camera_key_t]] | An event fusion's track per component and its keys |
| `g_camera_script_variable_indices` | The script variables the event camera reads and writes (the yaw, pitch, zoom and position) |

## Main flow

1. **Each frame** [[battle_state_run_game_loop]] works out the following step
   ([[battle_camera_update_vector_from_game_state]]), rebuilds the matrix and
   draws the map with it; after the state handler it pans, turns, zooms and
   tilts ([[battle_camera_update_map_pan]], [[battle_camera_update_rotation]],
   [[battle_camera_update_zoom]], [[battle_camera_update_tilt]]).
2. **The states** read L1/R1/L2/R2 and decide whether the d-pad pans
   (`g_battle_state_camera_pan_enabled`).
3. **An event frame** ([[battle_script_run_event_frame]]) copies the camera
   into the script variables
   ([[camera_store_state_to_script_variables]]), lets the CAMERA
   instruction's thread move them, and copies them back
   ([[battle_camera_load_state_from_script_variables]]). The script waits for
   the camera with [[camera_wait_until_idle]], and the End instructions
   settle the tilt and zoom ([[camera_init_tilt_and_zoom]]).
4. **An effect** starts its tracks' keyframes on the frames the timeline names
   ([[battle_camera_start_parent_keyframe_moves]],
   [[battle_camera_start_child_keyframe_moves]]) and advances them each frame.

## Where to change

| To change | Edit |
|---|---|
| The turn speed | [[battle_camera_update_rotation]]: the increment grows by twice the animation speed up to 0x30 |
| The zoom levels and speed | [[battle_camera_update_zoom]], [[battle_camera_toggle_zoom]] (ONE and 0xc00) |
| The tilt angles | [[battle_camera_update_tilt]], [[battle_camera_toggle_tilt]] (0x12e and 0x1c0) |
| Panning | [[battle_camera_update_map_pan]]: 0x4000 a frame, clamped four tiles outside the map |
| Following the cursor | [[battle_camera_update_cursor_tile_vector]] (the window that leaves the camera still) and [[battle_camera_step_focus_toward_cursor_tile]] |
| Turning away from hidden tiles | [[battle_camera_rotate_when_unit_tile_not_visible]] and the tiles' camera-block bits |
| An event camera move | [[camera_thread]] (interpolation modes and curves) |
| An effect's camera | The three `battle_camera_execute_*_command` handlers (their source modes) |
| Which sprite draws in front | [[battle_camera_get_tile_sort_point]], [[battle_camera_get_step_sort_point]] and their offsets of 1 and 5 |

## Quirks and debts

- The timed camera moves never run: only the uncalled
  `battle_camera_set_*_delta_*` setters start them, though the action, event
  and map-change states step them every frame.
- [[battle_camera_start_zoom]] and [[battle_camera_start_tilt]] have no
  caller; L2 and R2 go through the toggles.
- The yaw a turn starts from (`g_battle_camera_previous_yaw`) and the yaw
  band ([[camera_store_yaw_band]]) are written and never read.
- [[camera_pick_nearest_unblocked_yaw]] with all four sides blocked
  moves the yaw variable by 0x8000.
- [[battle_camera_get_input_direction]] leaves its input unset for an
  out-of-range mode.
- [[camera_store_state_to_script_variables]] stores the three zoom
  scales in one script word.
- WORLD's copy of the event camera never runs, and its wait calls
  BATTLE's [[battle_camera_is_active]], whose address holds WLDCORE code while
  WORLD runs ([[camera_wait_until_idle]]).

## Functions in scope

![[Camera scope]]
