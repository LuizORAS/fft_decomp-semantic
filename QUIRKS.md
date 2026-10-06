# Target quirks

Non-obvious facts about the retail code. They explain source that looks wrong
and mark code that a cleanup must not "fix". Details live in the named file.

## Retail bugs the source reproduces

- `DrawOTagEnv`: the diagnostic string uses literal `&08x` for its second item, so it does not format the environment pointer.
- `StCdInterrupt`: without `CdlModeSize1`, it skips the local disc-position read but still copies the uninitialized four bytes into the ring descriptor.
- `sprintf`: unchecked precision and zero padding can overrun its 512-byte temporary segment buffer.
- `src/psyq/libgpu/FntPrint.c`: unsupported conversions leave the field length undefined; the count limit is checked after writing, and oversized hexadecimal widths can overrun the 512-byte temporary buffer.
- `battle_menu_build_idle_action_menu`: passes one `s16` to `battle_unit_project_misc_to_screen`, which stores x and y; the y store lands in stack padding, and `column` tests the projected x.
- `DecDCTvlc`: the pause path saves its Y predictor at `0x80073f34`, overwriting the first four bytes of the overlapping birthday month-length table.
- EFFECT `effect_eNNN_update_*` renderers: `battle_effect_step_emitter_timeline` sets phase 3 when a callback's keyframe window has two frames left, even if init never ran; the destroy path then reads its latch through the null work slot (kernel RAM).
- `src/battle/battle_camera_store_state_to_script_variables.c`: all three zoom
  components are stored to script word `0x20`, so the first two are lost.
- `battle_script_focus_speed` accumulates into an uninitialized `$s6`: the event
  dispatcher leaves `operand_3` there, while camera fusion leaves its key count.
  Those caller values affect the camera duration.
- `src/battle/battle_script_toggle_message_portrait_flip.c`,
  `src/world/world_script_toggle_message_portrait_flip.c`: the record index is never
  assigned; all six passes use whatever `$s0` held. Keep the local
  uninitialized.
- `src/world/world_menu_resize_parent_entry_to_digits.c`: calls
  `world_text_count_decimal_digits` with no argument; the callee reads a stale
  `$a0`.
- `src/wldcore/wldcore_proposition_determine_success.c`: the preferred-job
  check resolves the party unit once, before its loop, from the scoring loop's
  leftover (past-the-end) index, so it tests that one unit every pass.
- `src/battle/battle_map_calculate_slope_height.c`,
  `src/battle/battle_calculate_screen_z_from_input_coords.c`: an off-map point
  (a unit staged outside the map, its screen coordinates read unsigned) gets no
  tile, and the null tile is read from RAM address 0. The unit shadow, palette
  modulation and per-frame rotation code (`battle_gfx_draw_unit_shadow`,
  `battle_gfx_calculate_sprite_shadow_from_tile_slope`,
  `battle_gfx_apply_misc_unit_palette_modulation`,
  `battle_gfx_update_all_unit_rotation_and_vectors`) do the same for units
  that chapter-end cutscenes walk off the map.
- `src/battle/battle_move_animate_fall_to_target_tile.c`: the event-state
  (`0x34`) path writes map Y into `real_z` (not `real_y`); the height branch
  then overwrites it.
- `src/battle/battle_move_update_walking_step_at_tile_edge.c`: adds the
  destination tile's water standing offset to the current edge height instead
  of the destination's, so for a unit that floats on, walks on or moves in
  water the step test sees a water destination lower than its surface, not
  higher: a step up onto deep water can take a plain walk where a climb hop or
  jump is due.
- `src/battle/battle_map_light_state_command.c`: several arms return an
  uninitialized pointer.
- `src/battle/battle_target_clear_panels_on_untargetable_tiles.c`: visits only the lower layer's 256
  panels, so an untargetable upper-layer tile stays in range of a single-tile or DIRECT_TARGETING
  ability (the weapon range's `battle_target_clear_range_on_untargetable_tiles` covers both layers).
- `src/battle/battle_target_mark_unit_panels_by_team.c`: its loop never advances the unit pointer,
  so it marks only the caster's tile; no retail ability sets the flags that call it
  (`ABILITY_SECONDARY_FLAG_1_ALLY_UNIT_TILES`, `_ENEMY_UNIT_TILES`).
- `src/battle/battle_menu_preview_attack_caster_stats.c`: for an item the
  secondary-data pointer stays uninitialized, yet its `flags_3` is read before
  the item test.
- `src/battle/battle_map_load_mesh_variant.c`: scales the mesh header's color
  palette byte offset by four, placing the palette read past most mesh files.
- `battle_map_queue_textured_triangles`, `battle_map_queue_textured_quads`
  and their untextured twins pack SXY with the full signed MAC1 word; a
  negative X therefore forces the screen Y halfword to `0xffff`.
- `src/battle/battle_map_blend_ambient_light_color.c`,
  `src/battle/battle_map_blend_darkness_color.c`: modes `11` and above leave the
  target colour uninitialized (no default case).
- `src/battle/battle_ai_load_ability_entry.c`: the CT adjustments test the
  Monster Skill (`0x80`) and Defend (`0x40`) support bits; Short Charge and
  Non-Charge were presumably intended.
- `src/battle/battle_ai_evaluate_status_cancellation.c`: the Blood Suck branch
  tests decision bit `0x04`, which is built from Jump.
- `src/battle/battle_ai_build_monster_skill_tile_mask.c`: the `-1` neighbour
  offsets are loaded unsigned (+255), so only `+x`/`+y` survive, and the level
  loop runs for level 0 only.
- `src/battle/battle_ai_build_targetable_tile_mask.c`: radii 8–15 index past
  the 8-entry template table into the following status weights.
- `src/battle/battle_ai_evaluate_reflected_target_origins.c`: the resume entry
  skips the reflector pointer reload and depends on the caller's `$s2`.
- `src/battle/battle_ai_evaluate_math_targets.c`: the extra known-ability call
  in the special-ID loop discards its result.
- `src/battle/battle_formula_apply_ability_element.c`: Oil doubles XA against fire, but every
  caller has already stored XA * YA as the HP damage, so Oil never raises fire damage (it is still
  marked for removal). Players know this as Oil having no effect in the PlayStation version.
- `src/battle/battle_action_init_current_ability_strike_data.c`: the range-data
  guard `ability_id >= 0 || ability_id < ABILITY_ID_ITEM_FIRST` is always true
  (`&&` was presumably meant), so item ids read past the table; only the
  default-menu Item skillset `0xa1`, which no job or ENTD unit uses, holds them.
- `src/battle/battle_action_run_pre_formula_setup.c`: both range-data bounds are
  `ABILITY_ID_ITEM_FIRST + 1`, so Potion (`0x170`) copies the item-ability item
  ids past the table as its range data; the Item menu clears it for consumables.
- `src/world/world_item_sort_id_list.c`,
  `src/event/equip_item_sort_list_by_criteria.c`: the evade sort keys treat
  `0x7a`..`0x8f` as shields and `0x90`..`0xef` as accessories, so throwables
  index the 16-entry shield table with weapon ids (122..127) and headgear and
  body armor index the accessory table with helm/armor ids; those items sort
  by unrelated bytes. A fix splits weapons at `ITEM_ID_SHIELD_FIRST` and keys
  headgear and body armor, which carry HP/MP bonuses instead of evade, as 0.
- `src/battle/battle_camera_step_real_coords_toward_target.c`: in the positive
  direction the Y (`vz`) step adds the vector component twice; X and Z add it
  once.
- `world_menu_open_entry_window` receives requested x/y origins but always
  opens at its fixed default origin.
- `src/battle/battle_gfx_build_next_action_result_display.c`: the CT-zero
  display case reads `y_shift` without setting it; only the level-up/-down
  cases assign it, so the sprite row depends on the leftover value.
- `src/main/main_unit_calculate_random_equipment.c`: throwing and bomb item
  types index past the four equipment-category bytes into the first innate
  status byte; that adjacent byte controls their eligibility.
- `src/event/jobstts_text_render_encoded_ids_to_image.c`,
  `bunit_text_render_ids_into_image.c`, `equip_text_render_encoded_ids_to_image.c`
  and `card_text_render_encoded_ids_to_image.c`: the encoded-text branch tests
  its glyph counter (`glyph_index`, `col`) against the line width but never
  assigns it; the stale register stays below the width, so text never wraps.
- `src/battle/battle_unit_init_misc_data.c`: tints the new unit's palette
  before it clears `mount_state`, so a misc slot last held by a rider looks up
  a mount that is gone and reads its screen data through NULL (kernel RAM).
- `src/battle/battle_menu_display_projected_action_effect.c`: a skipped
  preview leaves `g_battle_menu_preview_target_action` null, and its
  `attack_accuracy` is read (from kernel RAM) before the null test.
- `src/world/world_formation_rebuild_unit_list.c`: the Soldier Office (shop
  0x65) lists only monsters, yet `world_formation_init_menu_state` stages unit
  0 even when that list is empty; with no earlier list since WORLD loaded, the
  pointer is 0, so the "restore HP/MP" stores copy kernel RAM 0x10/0x16 to
  0x0c/0x12.
- `src/battle/battle_ai_choose_wait_facing.c`: when the target shares the
  acting unit's tile, `battle_ai_find_direction_of_target` returns
  `BATTLE_AI_DIRECTION_OVERLAP` (4), which indexes `viable_directions[4]`: the
  never-written `past_viable_directions` byte after the array holds whatever
  the stack held and decides whether 4 is returned as the facing.
- `battle_menu_run_scrolling_ability_list_thread` and its WORLD twin
  `world_menu_scrolling_list_thread`: `SetSemiTrans(&frame, 1)` passes the
  address of the `frame` pointer, so the semi-transparency bit lands in a stack
  byte past the pointer and the sprite is unchanged.
- `src/event/helpmenu_run_battle_help_menu.c` and its REQUIRE twin
  `src/event/helpmenu_menu_run_require_help.c`: the vertical cursor writes its
  shadow's coordinates through `vert_poly[2]`, past the two-entry
  `cursor_polys`; the stack places `shadow_polys` there, so the writes reach the
  polygon `vert_shadow_poly` addresses. One four-entry array does not reproduce
  the two separate stack addresses.
- `main_heap_alloc` and `main_heap_alloc_smd` tag a new block one above the highest tag in
  the cells before it, so it can share a tag with the block right after it (allocate 62
  cells, then 2; free the first; allocate 10, then 52: the last two blocks are both tag 2).
  `main_heap_free` clears cells while the tag repeats, so freeing the 52-cell block frees the
  2-cell one too, whose own free then returns 0. No retail sequence is known to do this.
- Unchecked divisions whose divisor can be zero; the R3000 `div` does not trap
  and leaves quotient -1 (1 for a negative dividend) and the dividend as the
  remainder: `100 / ct` in `bunit_ability_get_ct_display_value`,
  `jobstts_ability_get_ct_display_value` and
  `world_ability_get_ct_display_value` (CT 0 displays -1 + 1 = 0);
  `0x20000 / battle_effect_lerp_linear(...)` for the wave steps in the eight
  `effect_eNNN_update_wave_mesh_state` copies (E033, E035, E073, E079, E080,
  E230, E453, E456); and the direction-times-spread products over `scale` in
  `battle_effect_spawn_particle_motion`.
  More generally, GCC 2.6.3 turns a division by a constant into a multiply,
  so nearly every `div`/`divu` in the game divides by a variable: 1192 in
  game code, of which 84 carry the compiler's zero check (`break 7`) and 33
  follow a branch on the divisor. The other 1060, in 109 distinct functions
  (726 in five routines copied across the EFFECT overlays), rely on the
  divisor never being zero or on the R3000 result when it is. A native build
  must give each the R3000 result through a helper or guard it: x86 raises
  SIGFPE and ARM64 returns 0.

## Calls that disagree with the callee

Some calls need an erased signature or a function-pointer cast to preserve
their register values. Others can declare ignored parameters and call directly
without changing the bytes.

- `StCdInterrupt`: emulated header/payload copies pass a fourth completion argument to the three-argument `mem2mem` helper, which ignores it.
- `g_battle_thread_call_target` is the main-stack dispatch slot for callees
  with different signatures; assignments erase their function types.
- `world_card_build_save_slot_description` uses the job-name pointer that the
  void `world_gfx_bind_data_pointer` leaves in `$v0`; a native build must
  return it.
- `battle_thread_resolve_id`, `battle_thread_resolve_id_after_current`,
  `world_thread_resolve_id` and `world_thread_resolve_id_after_current` return
  the leftover `$v0` of the void `battle_thread_exit_current` or
  `world_thread_exit_current` when no thread slot is free (the WORLD source
  falls off its end). The caller has exited by then, but the scheduler
  resumes slot 0 without testing it, so a main-loop caller gets that value
  on the next frame.
- `battle_move_calculate_walkto_pathing` has no return statement;
  `battle_move_start_unit_walk_to` uses the path that `battle_move_calculate_pathing` leaves in
  `$v0`, which a native build must return.
- `battle_map_load_gns_and_move_find_items` has no return statement for a map whose GNS sector is
  0 (map ids 120-124, 126 and 127 in the retail table); `battle_map_load_data` compares the
  leftover `$v0` of the BIOS `bzero` with `g_battle_map_gns_records`. A native build must return
  a value there.
- `src/battle/battle_reaction_try_distribute.c` passes the unit twice to the one-parameter
  `battle_reaction_fails_brave_roll` through a function-pointer cast; the second argument is ignored.
- `src/battle/battle_formula_calculate_critical_hit.c` calls the argument-less
  `battle_formula_calculate_knockback` through a cast with XA's address in `$a0`; the callee ignores it.
- `src/battle/battle_formula_apply_ability_element.c` calls the argument-less
  `battle_formula_nullify_action` through a cast with `mount_info` in `$a0`; the callee ignores it.
- `src/event/equip_unit_load_selected_data.c` passes two arguments to
  `equip_unit_copy_data_to_compare_slot`, which takes none.
- `src/world/world_menu_resize_parent_entry_to_digits.c` passes none to
  `world_text_count_decimal_digits` (see above).
- `src/world/world_text_render_id_list_to_image_rows.c` passes a ninth stack
  argument that `world_text_render_ids_into_image` never reads. The BUNIT twin
  instead declares it as `unused_flags` in `include/fft/event_bunit.h`; unify the two.
- `src/battle/battle_move_animate_fall_to_target_tile.c` passes a coordinate
  buffer to the one-argument
  `battle_gfx_calculate_screen_z_from_misc_screen_data`, as it does to
  `_with_caller_data`; `battle_move_animate_jump_arc_to_own_tile.c` does not.
- `g_main_smd_modulator_waveforms` entries 8-15 call the void
  `main_smd_modulator_deactivate`; `main_smd_update_modulators` adds the
  leftover `$v0` (the cleared modulator flags) to the channel output.
- `main_sound_update_tunes` passes `current_music` to `SuzukiGetMusicPlaying`
  even when no scenario music is loaded; the null record reads the status
  halfword at address `0x10`.
- `src/world/world_menu_animate_window_quad_crop.c` passes an uninitialized
  source (a stale `$s5`) to `world_menu_zoom_cursor_frame`, which never reads it.
- `src/event/equip_cmd_run_stream.c` calls each `g_equip_cmd_handlers` entry
  without an argument; `$a0` still holds the stream.
- `src/event/equip_entrypoint.c` calls `main_unit_refresh_stats_and_statuses`
  without an argument; `$a0` still holds the unit's stats.
- `g_equip_menu_list_row_callbacks` holds `s32 (*)(s32)` entries, but slots 2
  and 3 are `equip_item_build_row_icon_rect` and
  `equip_item_build_row_graphic_descriptor`, which return record pointers.
- `src/event/bunit_cmd_run_stream.c` calls each `g_bunit_cmd_handlers` entry
  without an argument; `$a0` still holds the stream.
- `battle_unit_get_screen_data_ptr_by_misc_id` returns NULL for a missing
  unit, but its callers test for -1 and read through it. A teleport-out with
  removal (Wiegraf at the Fovoham windmill) fades the unit out while the
  teleport particles still read its screen data, from kernel RAM at address 0.
- `battle_map_get_tile_data_ptr_from_battle_id` and `_from_misc_id` read the
  unit's map position without checking the lookup, so a unit with no misc
  record (an arc-trajectory target in a Chapter 2 battle) is read from kernel
  RAM at address 0.
- `battle_menu_build_unit_portrait_poly` reads the speaker's battle record
  through `battle_unit_get_stats_from_battle_id`, which returns NULL for a
  speaker who is not on the field; the portrait test then reads kernel RAM.
  `battle_menu_copy_unit_data_to_status_billboard` does the same while
  counting units, for a misc record with no battle unit (the final battle).
- `world_item_sort_id_list`: it counts each unit's five equipment ids into a
  256-byte local array, but the ids are halfwords and an item tried on in the
  shop's fitting room carries flag bits above the low byte, so sorting the
  list then increments a byte far past the array.
- `battle_get_misc_id`: for the active-turn selector 0x69 with no unit
  holding the turn, it falls back to ENTD units 1 and 2 and tests the Jump
  status of whatever the finder returned; with neither on the field that is
  NULL, so it reads kernel RAM at address 0x58.
- `src/battle/battle_unit_init_misc_data.c`: stores the spritesheet-slot
  claim (0xffff when all nine slots are taken) in the u8
  `spritesheet_vram_slot` and tests the stored 0xff against 0xffff, so the
  failure path never runs and a tenth sheet loads into slot 255.
- `src/battle/battle_ai_load_ability_entry.c`: a Jump ability's CT estimate
  for the acting unit reads `acting_unit`, which is NULL during the AI
  workspace setup at battle start (`acting_unit_id` is 0), so slot 0's Jump
  divides kernel-RAM bytes.
- `src/event/bunit_text_concatenate_ids.c` passes the text section's address as
  an `s32` (and a third argument) to `bunit_text_skip_encoded_segments`;
  `equip_text_concatenate_ids.c` does the same to its EQUIP twin.
- `jobstts_cmd_run_stream_with_mode` takes the input word through its `void*`
  second parameter.
- `src/wldcore/wldcore_bar_handle_menu_input.c` passes the Bar's level record to `wldcore_menu_pop_level_and_rebuild_screen`, which takes no arguments.
- `wldcore_window_build_yes_no_panel` takes its origin record by value; `wldcore_proposition_handle_accept_input` and `wldcore_list_handle_completed_propositions_input` call it through a six-word cast with x and y in `$a0`/`$a1`.
- Calls that omit arguments the callee reads take them from whatever the
  registers hold; a native build must pass them explicitly:
  - the three `battle_camera_get_input_direction` callers pass only `mode`;
    `input` is the leftover `$a1` when `g_battle_controller_input` is not 0/1;
  - `battle_camera_call_toggle_tilt` passes nothing to
    `battle_camera_toggle_tilt`, whose tilt-target-1 path plays its sound
    with the leftover `$a1`;
  - the five `battle_effect_init_data` callers pass nothing, so the states
    it does not handle return the leftover `$a0`;
  - `bunit_gfx_build_item_graphic_descriptor` and
    `equip_gfx_build_item_graphic_descriptor` call
    `battle_get_item_graphic_data` without the item id (`$a1`);
  - `battle_map_init_units_sprites_event_and_music` and
    `battle_map_step_init_sequence` call the background-gradient,
    ambient-light and darkness initializers without `map_id` (`$a2`), and
    `main_sound_stop_sfx` without its sound id (`$a0`);
  - `battle_status_resolve_unit_changes_in_preview` (referenced nowhere on the disc) calls
    `battle_status_resolve_unit_changes` without `unit_id`/`removal_only`;
  - `world_script_is_deployment_running` calls `world_script_run_frame`
    without `ot`/`buttons`.
- These calls rely on the caller's own incoming registers instead, which
  hold the right values: `equip_thread_start_if_idle` (thread id in `$a0`)
  for `battle_thread_is_running`, `battle_unit_start_post_attack_animation_display`
  (both arguments) for `battle_unit_set_target_animation_from_attack_type`, and
  the EQUIP selection wrappers (`input_mask` in `$a2`) for
  `equip_menu_update_wrapped_horizontal_selection` and its vertical twin.

## Declaration leads

- `include/fft/menu.h`: `world_menu_entry_t.window_x` is `u16`, but
  `src/world/world_menu_open_entry_window.c` needs it signed.
- The unit status record (`battle_unit_status_record_t`, `include/fft/battle.h`,
  signed gauges) is redeclared as `world_unit_status_billboard_t` (WORLD,
  ATTACK, REQUIRE; unsigned HP/MP), the head of `equip_unit_data_t` (signed:
  EQUIP compares the 999 caps signed), and HELPMENU's raw
  `g_helpmenu_active_banner`.
- `equip_gfx_context_t` (`include/fft/event_equip.h`), `jobstts_gfx_context_t`,
  `bunit_gfx_context_t` and `world_gfx_packet_buffer_t` share one 25-pointer
  pool layout (BUNIT and WORLD match through `0xec`); their pool names disagree.
- `g_wldcore_zodiac_start_dates[12][2]` (`include/fft/wldcore.h`) and OPEN's
  `g_open_birthday_zodiac_start_dates` are the same `{month, day}` table; the
  OPEN copy stays a flat `[24]` because the `[12][2]` spelling changes its
  reader's code.
- `battle_unit_misc_data_t.movement_value` (`+0x11c`, `include/fft/battle.h`)
  is a plain `u8` holding a packed step (`battle_move_step_bits_e`): direction in
  bits 6–7, layer in bit 5, length in bits 0–1; its readers shift and mask it by hand
  (`src/battle/battle_move_get_current_and_destination_tiles.c`).
- `g_world_gfx_full_texture_window` is a `RECT`, but
  `src/world/world_formation_build_view_primitives.c` reads its first 4 bytes
  as a screen point.
- `g_main_item_location_flags` (`0x80059414`): only the first 64 bytes (512
  Move-Find bits) are proven; the next 64-byte bank is saved with it but has no
  known meaning.
- `world_menu_init_quad_from_record` and `world_gfx_init_image_loading` take
  `u16*`/`POLY_FT4*` but handle a `RECT`, an image record and either a
  `POLY_FT4` or a `SPRT` (code `0x64`, whose `w`/`h` overlap `x1`/`y1`);
  `world_main_menu_text_window_thread` passes a `SPRT` through a cast. A
  typed version needs a primitive union or separate SPRT/POLY_FT4 paths.
- The menu record is declared three ways: `world_menu_entry_t`,
  `world_menu_icon_thread_param_t` and `battle_menu_idle_action_entry_t`;
  `world_menu_run_thread` and `battle_handle_menu_cancel_input` take `void*`
  because their callers pass all of them.
- `void*` parameters whose callers pass another view of the data:
  `battle_gfx_set_draw_mode_from_rect` (a window record for its `mode0`
  `DR_MODE`), `battle_map_store_selected_tile_coordinates` (an `s16[3]` as an
  `SVECTOR`; only three halfwords are written), `equip_menu_init_scrollable_list_core`
  (a `u8*` text section read as halfwords), `open_gfx_load_opntex_into_frame_buffer`
  (OPNTEX bytes read as words), `world_gfx_build_scaled_draw_area_pair_at_offset`
  (the numeric editor's portrait packets as a draw-area pair),
  `world_menu_build_sprite_page` and its BATTLE twin (a `RECT` as an image
  location), `world_menu_init_and_load_scrollable_list` (menu scripts as a list
  record), `battle_target_build_directional_attack_panels` (a
  `battle_ai_command_action_t`, copied into a file-local duplicate of it),
  `equip_text_render_encoded_ids_to_image` (a `u32[]` image buffer) and
  `world_menu_run_script_with_palette_mode` (display scripts declared as
  `u8[]`, `s16[]` or `world_menu_window_command_t`).
- `jobstts_menu_init_scrollable_list` passes its text-table pointer to the
  core's `s32` parameter; pointers stored in 32-bit integers break on 64-bit
  ports.
- `battle_unit_misc_data_t` (`include/fft/battle.h`) repeats `battle_strike_work_t` field by field
  at `0x18c`..`0x1b3` (the strike work's last four bytes are the misc record's `action_rewards`);
  `battle_action_start_strike` and `battle_action_resolve_ability_strike` reach it through a cast
  of `&misc->action_18c`, and `battle_action_apply_strike_results` through another.
- `0x80165ef4` carries two names (`g_battle_text_substitution_values`,
  `g_dead_unit_roster_id`) because it holds several identifier kinds; keep
  its name generic.

## Pointers held in 32-bit integers

The PS1's pointers and `s32` are both 32 bits wide, so some code passes or
computes addresses as integers. A 64-bit build must give these pointer types.

- The thread parameters (`native_thread_t.function_parameter_1`..`_4`, and the
  `*_thread_get_current_parameter_*` getters) are `s32` but often carry record
  pointers, e.g. into `battle_script_run_sprite_move` and
  `world_menu_confirm_action_silently`.
- `bunit_menu_dispatch_with_override` and `equip_menu_dispatch_with_override`
  take a menu-record pointer as `s32`; `bunit_menu_init_scrollable_list_core`,
  `jobstts_menu_init_scrollable_list_core` and `jobstts_menu_init_scrollable_list`
  take a text-table pointer as `s32`.
- Matching spellings compute addresses through `(u32)` casts, e.g.
  `battle_script_get_variable_word_pointer_from_id` (offsets from
  `g_battle_script_variables`) and `battle_ai_load_known_ability_flag` (the
  learned-ability row).
- The other direction: `battle_map_init_background_gradient`, `_ambient_light`
  and `_darkness` pass `map_id` through the `u8*` parameter of
  `battle_map_light_state_command`.
- `wldcore_gfx_draw_projected_map_tiles` takes its `GsOT*` as an `s32` and
  converts it back for `world_gs_sortpoly`.

## Duplicated code

Twins stay separate functions because each lives in its own module or
translation unit. Share their types and constants through headers.

- EFFECT: 32 groups of byte-identical routines across 110 native files. E005's
  first routine appears in 17 overlays, and E450/E480 are identical.
  E336/E461, E230/E481 and E015/E047 differ only in immediates.
- BATTLE/WORLD pairs: scenario conditionals
  (`battle_script_run_scenario_conditions.c` /
  `world_process_scenario_conditionals.c`), action-slot restrictions, the
  portrait-flip toggle (instruction `0x37`), staged status, selected-tile data,
  and the event interpreters.
- `src/wldcore/wldcore_menu_push_script_flag_01a4_detail_level.c` and
  `src/wldcore/wldcore_menu_push_story_event_text_level.c` are one routine at
  menu levels `0x2f` and `0x1e`; the second reserves 8 unused stack bytes.
- `battle_map_blend_ambient_light_color.c` and
  `battle_map_blend_darkness_color.c` are one 1,032-byte function over
  different records.
- `world_gfx_reset_record_texture_window{,_2,_3}.c` differ only in the `RECT`
  they install.
- Overlays share load addresses, so an address match is not an identity
  match. Compare bytes: REQUIRE and EQUIP both have code at `0x801c2dcc`, and
  the two routines differ.

## Other surprises

- `main_gfx_load_efc_fnt`: patches the low immediate byte of `FntLoad` at `0x80022f5c`, changing its font CLUT placement from `y + 128` to `y + 127`.
- `StCdInterrupt`: tracks frame numbers through the header's low `u16`, while DMA completion and OPEN retain the full `u32` frame number.
- `psyq_gte_apply_rotation_ir`: writes saturated IR1–IR3 output words and returns its unchanged third argument.
- `sprintf`: unsupported conversions terminate output, `%#p` prefixes `0p`, hexadecimal alternate form prefixes zero, and `%#.0o` with zero emits no digit.
- JOBSTTS.OUT offset `0x0` and BUNIT.OUT offset `0x200` hold the literal `"%d"`
  (`g_jobstts_text_decimal_format`, `g_bunit_text_decimal_format`) used by `src/event/jobstts_menu_script_draw_formatted_number.c` and
  `src/event/bunit_cmd_draw_right_aligned_number_handler.c`.
- The game-options word packs its last five fields out of array order
  (`include/fft/menu.h`).
- CallFunction (`battle_script_execute_event.c`, `world_script_execute_event.c`)
  tests selectors in sequence against one operand that the arms modify. So
  `0x06` also runs the `0x0f` warp, and `0x0e` can fall into later arms.
- `src/battle/battle_reaction_check_when_targeted.c`: formula 7
  suppresses reactions. Counter Magic (`0x1b3`) is tested last, after byte
  `+0x8d`, so Counter (`0x1ba`) wins.
- `g_main_debug_display_enabled` is only ever cleared by retail code; the
  add-unit threads and unit-summary panels print debug output when it is set.
- `src/event/helpmenu_run_battle_help_menu.c`: an earlier close arm catches
  help mode 2, so its group is unreachable.
- `src/wldcore/wldcore_input_check_repeating_directional.c`: the arms test
  UP, DOWN, RIGHT, LEFT, R1, L1, but the counters sit in UP, DOWN, LEFT, RIGHT,
  R1, L1 address order. The early-out clears only the four direction counters.
- `src/wldcore/wldcore_map_roll_random_encounter.c`: encounter masks count from
  the high bit, so the chosen ENTD is `entds[7 - bit]`.
- `src/battle/battle_action_finalize_draw_out_katana_result.c`: when the
  katana breaks, the strike work's `can_earn_experience` is copied onto itself.
- `src/battle/battle_menu_run_icon_selection_loop.c` and its WORLD twin: the
  loop clears both records' `+0x78` words when they hold 0 and 2, but only the
  record builders write that word (always 0), so the reset never fires.
- `src/wldcore/wldcore_menu_step_treasure_detail_level.c` and its unexplored-land
  twin: phase 1 waits on the render record named by `+0x08` (`sound_novel_slot`),
  which neither push sets, so it tests whatever the previous level in that stack
  slot left there. Phase 3 waits on `render_index` instead.
- `src/psyq/libc/memmove.c`: the overlap-safe copy returns the original destination on its backward path and the advanced destination on its forward path.
- `StartRCnt` and `StopRCnt` index the IRQ-mask table before validating a counter: selector 3 changes the VBlank mask even though `StartRCnt` returns zero; `StopRCnt` always returns one. Larger indices can read past the four-entry table.
- `SpuGetVoiceEnvelopeAttr`: the public `s32*` key-status output is written with a halfword store; its upper half remains unchanged.
- `InitCARD` and `StartCARD`: this linked version calls `ChangeClearPAD(0)`, while later SDK descriptions use one for startup; retain the retail value.
- `_patch_card2`: exchanges five resident/BIOS instruction words, so calling it again reverses the exchange rather than repeating an idempotent patch.
- `SetDrawLoad`: initializes the upload header and rectangle but leaves pixel payload storage to its caller.
- `main_sound_get_largest_free_block`: reports the rounded gap including space needed for a new block header, rather than a directly usable payload size.
- `battle_gfx_load_spritesheet_into_vram_slot`: its fixed 0x30d4-byte copy from SPR +0x9200 extends beyond some original file-read extents and includes retained heap bytes.
- `battle_gfx_update_and_animate_unit_wep_eff`: when neither EVTCHR slot is free, one path tests `j` before setting it.
- `g_battle_effect_disc_entries` (`0x801b53e8`) stores whole-sector sizes, and entry 0 repeats entry 1: effect id 0 loads E001.BIN, so E000.BIN's code is never loaded through the table.
- `src/world/world_gfx_load_wldface_to_frame_buffer.c`: the WLDFACE rectangles (`g_world_wldface_vram_rects`) use x = 1664–1856; the GPU masks VRAM transfer coordinates, so the pages land at x = 640–832.
- `SuzukiSPUInitialiser`: `main_sound_init_sfx_music` reaches `main_smd_insert_music`, which disables and re-enables `g_main_root_counter_2_event` before the initialiser opens that event: handle 0 at boot, the handle `main_sound_quit` closed on a soft reset.
- `src/event/equip_menu_load_images_and_reset_lists.c` and `bunit_gfx_init_vram_and_start_fade.c`: store 16-colour CLUT rows into the 4-entry `buf1`; they run on into `buf2`, which the frame places directly after it (the source reads as two arrays, the stack holds one).
- EFFECT scripts never run six state handlers: E314's script names no file function, and E134/E242 `update_ring_stack_mesh_state`, E454/E455 `map_set_3d_objects_to_state_2_state` and E456 `map_step_freeze_state` sit in callback slots (script opcode 06) that no keyframe selector fires.
- `src/battle/battle_map_draw_mesh_and_weather.c`: when map command 0x96 (E458) ends, `battle_map_set_weather_texture_overlay(0x8b)` restores a non-snow rain or storm mid-draw; the mode-0x55 block then re-links `ft3` drops the 0x96 block already linked at OT+2, an ordering-table cycle that holds `DrawSync` until its 240-VBlank timeout.
- `src/world/world_card_run_menu_screen.c`: its 0xCC60-byte stack frame of packet pools reaches below 0x801ff000 into the top cells of MAIN's game heap (0x801df000-0x801ff000); it is safe only while those cells are free.
- `src/world/world_script_handle_tutorial_command_end.c`: two paths fall off the end of the s32 handler and return a stale nonzero `$v0` (the fade's result, or the 1 just stored), so the formation loop keeps running.
- `src/world/world_menu_draw_thread_status_indicators.c` and its BUNIT, ATTACK and EQUIP twins: while a unit-status banner thread (8 or 7; 13 or 14 in EQUIP) is being stopped its first parameter is 0, and the panel check reads that "display record" at RAM address 0.
- `src/open/open_script_draw_text_records.c`: passes OT entries by value (`otag[15]`, `otag[8]`) to `AddPrim`; after `ClearOTagR` each holds the physical address of the entry below it, so the credits fade, bands and glyphs link into layers 14 and 7 through the KUSEG mirror.
- `src/wldcore/wldcore_gfx_step_dissolve_image_upload.c`: the step-0 call (start the dissolve) has no return value of its own; `$v0` keeps its clearing cursor, the address one byte below the band-progress table. Its three callers ignore that result.
- `src/world/world_menu_build_icon_record.c`: for the Bar's send-unit list (header 0x10) the arrow x reads the "height" of the record at the panel's value pointer, `&g_wldcore_active_menu_value` + 6, which is the upper half of `g_wldcore_proposition_dispatch_days` (0).
- `src/battle/battle_unit_generate_treasure.c`: outside action execution (AI simulation, preview) it returns with no value, so `$v0` still holds `g_battle_action_state` and the crystal pickup result names item 1 or 2 as the treasure.
- `src/open/open_title_step_new_game_start.c`: no retail code sets step 5, which pops New Game and opens the Music Test (`open_menu_start_music_test_controller`, controller slots 7 and 8); only a poke of the step word reaches it.
- WORLD's copy of the battle menus is dead code: nothing in any module calls or names `world_menu_init_system_function` (0x800f5230) or `world_menu_start_mini_menu_display_thread` (0x800f0e48), so modes 1 (AT list) and 2 (dead-unit panel) of `world_menu_run_main_mode` never run; the world map's Options row runs mode 0.
- `src/battle/battle_get_misc_id.c`, `src/world/world_get_misc_id.c`: during
  Game Over event `0x194` every unit lookup resolves to Ramza, but no retail
  code, scenario chain, `BTLEVT.BIN` condition, event or world script starts
  `0x194`; the engine plays the byte-identical script at `0x190` without the
  redirect.
- `battle_heap_alloc_block` has no out-of-memory exit: when no free block is large enough it
  walks the circular free list forever.
- WORLD runs threads in slots 1-16: `world_thread_yield` wraps at 17, and `world_thread_resolve_id`
  and `world_thread_find_running_by_task` scan to 16 (BATTLE stops at 15). The array has room for
  the 17th slot (`g_world_gfx_texture_allocation_grid` starts right after it), but
  `world_thread_reset_scheduler` clears only slots 0-15, so slot 16 keeps its state across a reset.
- `battle_state_handle_change_map_jumping_in_state` turns event results 9 and 0xA into game flow
  0 and 1, the reverse of `battle_state_handle_event_state`, and ignores 0xB and 0x13, so a
  scenario that finishes during an event map change's fade-in would take the other exit (or
  none).
- `battle_turn_take_next_event` handles turn event 0x400 (`BATTLE_TURN_EVENT_UNKNOWN_0400`), but
  `g_battle_turn_event` only takes `battle_turn_run_clock`'s results, which never include it, so
  that arm never runs.
- `battle_move_start_next_post_move_event` and `battle_state_handle_crystal_learn_state` handle
  post-move event `0x200` (`BATTLE_MOVE_POST_EVENT_SOURCE_DISPLAY`, a display refresh of the
  mover), but `g_battle_move_post_move_events` only takes `battle_move_get_post_move_events`'
  results, which never include it, so that arm never runs.
- `src/battle/battle_formula_apply_weather_effects_on_bows.c` reads the weather script variable
  instead of `battle_map_get_effective_weather`, so a snowstorm also cuts bow and crossbow hits and
  the map's ignore-weather flag is not checked; the element modifiers use the effective weather.
- `src/battle/battle_formula_damage_pa_times_wp_plus_y_status.c` (Holy Sword) and
  `battle_formula_break_equipped_damage_pa_times_wp.c` (Might Sword) apply the Strengthen and
  affinities of the weapon's element only; the element in the ability's own data (Holy for the
  Holy Sword abilities) is never read.
- `src/battle/battle_menu_display_projected_action_effect.c`: the 0x100 bit that
  `main_status_find_action_highest_order_effect` adds for a status flagged
  `STATUS_EFFECT_FLAG_2_EVENT_EXCLUDED` changes nothing. The preview tests 0x80 (a removal) first, so
  its 0x180 test, which would pick marker image 0x1e, never passes.
- `src/battle/battle_unit_set_animation_based_on_status.c` picks animation 0x21 for bit 0x01 of the
  status mirror `status_flags_1_4`, but no status sets that bit
  (`g_battle_misc_status_mask_by_handler_index`), so the animation never plays.
