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
- `src/wldcore/wldcore_calendar_advance_days.c`: an event's date advance keeps the
  elapsed days of an active proposition that does not finish within it, so those days
  never count toward it (the daily advance counts each one). It also misses a
  birthday that falls after December 31 inside the advance, and counts the year at
  December 31 where the daily advance counts it on March 21.
- `src/wldcore/wldcore_proposition_determine_success.c`: the preferred-job
  check resolves the party unit once, before its loop, from the scoring loop's
  leftover (past-the-end) index, so it tests that one unit every pass.
- `src/wldcore/wldcore_sound_novel_calculate_completion_percent.c` (no caller): counts 0x1000 bits
  from `g_main_sound_novel_section_bits`, which holds 1280 (40 words), so it also counts the
  Move-Find item flags and most of the poached item quantities stored after it.
- `src/battle/battle_map_calculate_slope_height.c`,
  `src/battle/battle_gfx_calculate_screen_z_from_input_coords.c`: an off-map point
  (a unit staged outside the map, its screen coordinates read unsigned) gets no
  tile, and the null tile is read from RAM address 0. The unit shadow, palette
  modulation and per-frame rotation code (`battle_gfx_draw_unit_shadow`,
  `battle_gfx_calculate_sprite_shadow_from_tile_slope`,
  `battle_gfx_apply_misc_unit_palette_modulation`,
  `battle_gfx_update_all_unit_rotation_and_vectors`) do the same for units
  that chapter-end cutscenes walk off the map.
- `src/battle/battle_move_animate_fall_to_target_tile.c`: the event-state
  (`0x34`) path writes the tile-centre depth into `real.vy` (not `real.vz`).
  The first frame adds the fall speed to it, which puts the unit below the
  ground, and the next frame snaps it to the ground: an event fall ends on its
  second frame.
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
  (`ABILITY_SECONDARY_FLAG_1_ALLY_UNIT_PANELS`, `_ENEMY_UNIT_TILES`).
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
  tests `BATTLE_AI_DECISION_JUMPING`, built from Jump (bit `0x04` of status
  byte 0; Blood Suck is bit `0x04` of byte 1), so Protect allies skips Blood
  Suck on a target that is not jumping.
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
  `bunit_text_render_encoded_ids_to_image.c`, `equip_text_render_encoded_ids_to_image.c`
  and `card_text_render_encoded_ids_to_image.c`: the encoded-text branch tests
  its glyph counter (`col`) against the line width but never assigns it; the
  stale register stays below the width, so text never wraps.
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
- `src/world/world_formation_get_unit_palette_index.c`: `job` is assigned only for sprite sets
  0x80 and up. Of the story identities below 0x80, 28 (Delita in chapters 2-4, Zalbag, Dycedarg,
  Larg, Goltana, Elmdor and other enemies) map to a portrait of 0x78 or more, which indexes past the
  20-entry `g_world_monster_formation_sprite_table`, and a result of 0x3e or more reads the unset
  `job`. The characters who join the party map below 0x3e. The monster branch also ORs the job's
  portrait palette in at bit 10 and masks it off with `& 0xFF` at once.
- `src/battle/battle_ai_choose_wait_facing.c`: when the target shares the
  acting unit's tile, `battle_ai_find_direction_of_target` returns
  `BATTLE_AI_DIRECTION_OVERLAP` (4), which indexes `viable_directions[4]`: the
  never-written `past_viable_directions` byte after the array holds whatever
  the stack held and decides whether 4 is returned as the facing.
- `battle_menu_run_scrolling_ability_list_thread` and its WORLD twin
  `world_menu_run_scrolling_ability_list_thread`: `SetSemiTrans(&frame, 1)` passes the
  address of the `frame` pointer, so the semi-transparency bit lands in a stack
  byte past the pointer and the sprite is unchanged.
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
- Locals read before any assignment, so the value is whatever the caller left
  in a register: `battle_effect_init_data` tests `result` (`$a0`) for a load
  state outside 0-3, which nothing sets; `battle_camera_get_input_direction`
  tests `input` (`$a1`) when `mode` or the controller mode is outside 0-2;
  `main_status_set_action_state` leaves `status` unset for an action state
  other than 0 and 5-8 (its `switch` has no default; no caller passes one);
  `battle_map_dispatch_gns_resource` returns a local no path assigns (`$s8`);
  `battle_menu_get_cursor_bob_offset` and `world_menu_get_cursor_bob_offset`
  walk an unset table pointer for a mode other than 0 or 1; `battle_effect_summon_charge_secondary_handler` returns
  `result` (`$a1`) unset when the phase is not 1-3, as does
  `battle_effect_linear_projectile_secondary_handler`;
  `bunit_unit_get_current_stat`, `bunit_unit_get_max_stat` and
  `world_formation_get_unit_stat_by_mode` return `value` (`$a1`) for a stat
  mode outside 0-2; `battle_move_start_unit_step` leaves `facing` (`$a2`, the
  destination tile its callers pass) unset for a direction outside 0-3;
  `battle_map_load_saved_state_record` returns `next` (`$a1`) for record type
  0; `world_formation_run_set_ability_menu` indexes the entry ids with
  `cursor` when the list is empty; `world_map_destination_list_thread`
  uses `selected` when no entry matches the current item;
  `helpmenu_menu_run_battle_help` and `world_menu_run_battle_help` read
  `node`, `x` and `y` in the cursor loop when the text thread is already
  running on the first frame, and `navigation` when the unit view mode is
  not 1-4; `battle_menu_init_system_function` passes `ptr` on unset for
  SCENARIO_CHECK and CLOSE; `battle_gfx_init_numeric_sprite_display` leaves
  `display` unset for a section outside 0-2 (its caller passes 0-2);
  `battle_map_update_mesh_part_animations` indexes the instructions with
  `next`, assigned only when a keyframe expires in the same pass, for a part
  whose animation state is set and whose keyframe is 0;
  `battle_map_control_gte_background_color` returns `result` unset for DISABLE
  and RESTORE (every caller issues SET); `open_card_get_info_with_retries`
  returns `result` unset for a retry limit below 1 (its caller passes 2);
  `battle_sound_stop_weather_sfx` returns an unset `result` no caller reads.
- `world_card_read_file_chunked` sets `result` to -1 when the last read was
  short and then returns `total`; the test has no effect.
- `helpmenu_menu_run_require_help` and `world_menu_run_require_help` reach
  the shadow quads by indexing past the end of the local `cursor[2]`
  (`quad[2]`, `quads[buffer + 2]`), relying on `shadow[2]` following it on
  the stack; one four-entry array does not reproduce the two stack addresses.
  `entry`, `x` and `y` are assigned only on frames where text
  thread 0x10 is idle and are unset if it is already running on the first
  frame.
- The six `*_text_render_decimal_value` copies assign 1 on every rung of the
  minimum-width ladder, so the value is always placed as one digit.
- `etc_graphic_build_game_over_polygons`: `texture_mode` is the constant 1,
  so the 4-bit CLUT arm never runs.
- `open_movie_init_mdec_stream_state` stores the slice width (24) into the
  global `g_open_movie_mdec_stream_state.output_rect.w`, not through its
  `state` parameter; the one caller passes that global.
- `battle_action_init_current_ability_strike_data` tests
  `ability_id >= 0 || ability_id < ABILITY_ID_ITEM_FIRST`, which is always
  true.
- The ENTD 7-byte AI copy reads three bytes past `ai_target_xy` (0x25-0x27),
  into the two AI targeting flags and `ai_initial_enemy_flag`.
- EQUIP never fills the physical accessory evade slot of `equip_unit_data_t`
  (0x48); it stores the accessory's magic evade to 0x52.
- `battle_status_modify_inflictions` masks five status bytes with the 4-byte
  table at 0x8018f60c; the fifth mask is the low byte of
  `g_battle_formula_handlers[0]`.
- Event script, both interpreters:
  - Ten instruction values (0x14, 0x20, 0x34-0x36, 0x52, 0x67, 0xe1, 0xe2,
    0xe4) have operand lengths in the size table and no handler; the
    scanners step over them and the interpreter ends the script on them. No
    retail script uses them.
  - AddGhostUnit runs an empty four-pass countdown before its free-slot
    search; with no free misc slot it `continue`s without advancing or
    yielding, so the script thread spins.
  - BlockStart, AddUnitStart or CameraFusionStart with no matching End gets
    offset 0 from the finder and the script resumes at byte 1.
  - FocusSpeed and RelativeCamera patch the next instruction's operands
    without checking that it is Camera; Focus scans for the next Camera with
    no End check.
  - UnitAnimRotate tests the animation id's high byte as its flag, never
    reads its last operand byte, and has no absent-unit check, so misc id
    2000 indexes the rotation table.
  - WaitUnitReady waits on `battle_return_one`, so it never waits.
  - Effect stores its x and y operands to two globals nothing reads.
  - DebugPause prints "PAUSE", yields one frame and sets a word nothing reads
    (`battle_script_print_debug_message`); instruction 0xdc stores 13 to
    another. Neither changes the script's course.
  - In WORLD, WalkTo is an empty arm outside a block, Arrow resolves both
    units and never sets the trajectory, and ChangeMap calls an empty
    function where BATTLE resets the script light transform.
- `wldcore_menu_step_location_menu_level` opens the outfitter with shop id 0
  for every town; `world_shop_run_screen` then takes the id from the
  location variable. The fur shop and soldier office pass their own ids.
- `wldcore_proposition_step_result_level` clears the in-progress bit with
  `& 0xFFFD` on an `s32`, which also drops the variable's high half.
- OPENING's runtime flags 8, 0x20000 and 0x80000 (`open_system_runtime_flag_e`: RENDER_RECORDS_HIDDEN,
  CLEAR_FRAME and SOFT_RESET_DISABLED) are tested and never set.
- Suzuki driver: music status bits 0x20 and 0x2000, channel flag 0x10 and
  driver flag 0x2000 are tested and never set; channel flags 0x80 and 0x4000
  and voice-attribute bits 0x7000 are set and never tested;
  `main_smd_set_flags_all_channels` and `main_smd_dispatch_snapshot` have no
  caller.
- CARD save menu state 9 is compared and never stored. EQUIP refuses command
  row 1, so its screen state 3 is never entered. `g_etc_graphics` entries 0, 5
  and 6 all name CHAPTER1.BIN.
- `world_script_reset_event_variables_thread` does `g_main_system_flags += 3`
  on a bit-flag word. System flag bits 0x80, 0x400000 and 0x20000000 are
  tested and never set.
- `card_save_run_menu` and `world_card_run_menu_screen` end with
  `set_parameters(2, 0, -1, 0)`: the backdrop thread exits only when its
  third parameter is set, so that call does not stop it. CARD also tests and
  stops slot 14 and suspends slot 15, which nothing in the overlay starts.
- `wldcore_menu_step_map_path_level` plays sound id 0 when the path
  animation ends; that entry has no streams, so it only stops the channel
  pair.
- The `*_text_render_decimal_value` copies read the digit count as the low
  nibble for alignment and as the low byte for the digit loop.
- Scenario conditions 18 and 19 (casualties) compare with the record's `id`
  halfword, not `value` as the war-funds pair does.
- ATTACK fills deployment squad ids 0x35/0x36 and clears 0x37/0x38, although
  the deployment screen handles four squads.
- Words 0xab-0xff of the script variable block are reachable by no variable
  id, yet word 0xff gates the BATTLE call in
  `main_overlay_call_battle_return_zero`, which itself has no caller.
- Formation menu states 22 and 23 have handler table entries and code paths
  and no code stores them. Menu input mode 1 is tested in four places and
  never stored.
- `battle_menu_wait_thread` and `world_menu_wait_thread` store -2 over the
  Wait entry's row action (7, `BATTLE_MENU_ID_WAIT_YES_NO`) before confirming,
  so the Yes/No entry of the menu table never opens. No code, row action or
  system function opens entries 0x09, 0x0d, 0x16, 0x17 (the only way to 0x18)
  or the four no-act turn menus (0x20, 0x21, 0x32, 0x33), yet the confirm-move
  threads reject Move for 0x21 and 0x33.
- `world_item_build_category_list` and `world_shop_build_item_list` treat
  filters 0 and 1 (the two hand slots) identically.
- The fur shop's "sell" list sells ordinary inventory at half price; its
  "buy" list is the poached stock.
- `main_unit_learn_job_abilities` sets the learned bit twice when the JP
  purchase succeeds: once in the purchase branch and again under
  `if (learned)`.
- `main_unit_apply_level_growth` calls `rand()` and discards the result; the
  call only advances the generator.
- `battle_gfx_load_misc_unit_into_evtchr_slot` searches
  `g_battle_gfx_vram_slots[0..1]` for a free load marker and decompresses into
  that slot's image data, but writes the owner, marker and state of the
  caller's slot.
- `require_party_save_battle_units_and_find_join_candidates` searches the formation sprite groups
  for the unit's row, remaps 0x49 to 0 and 0x47 to 0x48, and never uses the
  result.
- `require_overlay_open_jobstts` reserves 0x1f000 bytes (62 cells) at the game heap's base before
  loading JOBSTTS.OUT, but BATTLE's companion overlay 7 reads 0x20000 bytes there
  (`g_battle_menu_companion_executable_sizes`), so the top two cells (0x801fe000-0x801ff000) are
  overwritten without being claimed; JOBSTTS.OUT is 0x1b85c bytes, so they receive the disc bytes past
  its end. `battle_menu_run_jobstts_thread` reserves the full 0x20000.
- `helpmenu_menu_run_battle_help` and `world_menu_run_battle_help`: the
  `UNIT_VIEW_MODE_HOVERED_UNIT` arm and its navigation table cannot be reached
  (the first arm, which closes the help, takes modes 1 and 2); request
  kinds 4, 0x11, 0x12 and 0x17 call the close routine for an empty id and
  then start the text thread anyway, relying on close not returning.
- `main_heap_free`: the clearing loop stops at the first page byte that differs
  from the allocation id, not at the end of the 64-entry
  `g_main_heap_game_allocator_table`; an allocation that reaches the last page
  reads on into `g_main_gfx_draw_envs` and clears its bytes while they match.
- `battle_map_load_data`: the four texture quarter files load at
  `buffer + 0x8000 * (n + 1)`, while `battle_map_dispatch_gns_resource` uploads
  quarter n from `buffer + 0x8000 * n`; quarter 3 is read past the end of the
  0x20000-byte buffer.
- `battle_camera_toggle_tilt`: from tilt target 3 it starts a decrease toward
  target 4, which `battle_camera_update_tilt` has no branch to finish.
- `battle_action_run_pre_formula_setup`: both tests bound the default
  ability class at `ABILITY_ID_ITEM_FIRST + 1`, so the Potion ability indexes
  `g_main_ability_range_data` one record past its end, which is the bytes of
  `g_main_item_ability_item_ids`. The other callers bound it at
  `ABILITY_ID_ITEM_FIRST`.
- `battle_reaction_prepare_next`: Counter Tackle performs Dash under the
  Black Magic skillset, the skillset Counter Magic passes.
- `battle_formula_apply_ability_element`: Oil on a fire-element action doubles XA
  after every caller has stored the HP damage, and nothing reads XA again: Oil
  is removed and the damage is unchanged. The weapon-element path
  (`battle_formula_apply_weapon_element`) does not test Oil at all.
- `src/wldcore/wldcore_proposition_determine_success.c`: the level score is
  read from the bravery scores (proposition data record 5, three rows of
  five) with a ten-entry stride, so it runs on into the faith scores (record
  6). Record 7, three rows of ten level scores, is fetched by nothing: with
  job list 1 a level 91-99 unit scores 5 where record 7 holds 50.
- `src/battle/battle_ai_add_usable_skillset_abilities.c`: the Faith test reads the AI flags of the
  ability considered last (`g_battle_ai_data_base.considered_ability`), not of the one being added, so
  for an innately Innocent unit (the Steel Giant job) it keeps or drops all of a skillset's default
  abilities at once, by whatever that stale ability was.
- `src/battle/battle_target_validate_weapon_target.c`: the weapon-range flag
  is read from `g_main_ability_range_data[(u8)ability_id]`, so abilities
  0x100 and up use the record of their id's low byte.
- `battle_unit_apply_level_up_down_ability`: a level-1 target of a level-down
  (the Degenerator trap; formula 0x59 misses at level 1) is clamped back to
  level 1 and still loses raw / (growth + 1) of every raw stat.
- `main_sound_set_reverb_mode`: a negative mode is meant to keep the current
  mode, but the stored mode still carries the 0x100 work-area flag that LIBSPU
  strips from its own copy only. It never equals the SPU's mode, so there is
  no early return, and the reloaded value is treated as a mode of 10 or more.
  The 100 music files all hold mode 4.
- `g_main_smd_opcode_lengths`: the 37 unassigned opcodes have length 0, so
  the look-ahead of `main_smd_read_instructions` would never step past one,
  and 0x9D has three parameter bytes there while `main_smd_stop_sfx` consumes
  two. No retail sequence uses any of them.
- `main_smd_set_instrument`: the waveset kept on the Suzuki heap is the
  0xb30-byte header of WAVESET.WD (176 instruments), and most music channels
  end with Instrument 0xFF before End Bar, so entry 255 is read 0x4f0 bytes
  past the copy.
- `equip_item_sort_list_by_criteria` and `world_item_sort_id_list`: the evade
  keys read `g_main_item_shield_data` for ids 0x7a-0x8f and
  `g_main_item_accessory_data` for 0x90-0xef by `secondary_data_id`, so thrown
  weapons (122-127) read far past the 16 shield records, and headgear and body
  armour (0-63) read the 32 accessory records and on past them.
- `world_item_cash_out_excess_inventory` walks ids below `ITEM_ID_END - 1`, so
  Phoenix Downs (0xfd, the last item) over 99 are never cashed out; every
  other item walk runs to `ITEM_ID_END`.
- `battle_move_calculate_pathing` copies 256 terrain costs from row 0 of
  `g_main_terrain_movement_cost_tables` (64 a row) to the scratchpad cost
  block at 0x1f800100, on over the spread state at 0x1f800180. Its flag-0x100
  test (a WalkTo with use_terrain_costs clear) compares the cost-table index,
  not the cost, with 0xff, so blocked surfaces cost 1 too.
  `battle_move_store_unit_movement_to_scratchpad` copies 64 and keeps 0xff.
- `battle_formula_weapon_heal` (formula 0x07, the Healing Staff) applies
  zodiac compatibility twice, once itself and once inside
  `battle_formula_apply_physical_xa_modifiers`, whose Defense
  Up and Protect steps also cut the healing to two thirds.
- `battle_formula_raise_level_by_one_add_status_on_caster` (formula 0x57,
  Please Eat) stores a full HP and MP heal (max-HP damage for an undead
  target) and then overwrites `attack_type` with the level change's:
  `battle_action_apply_target_result` still applies the
  amounts, but `battle_gfx_prepare_post_action_display` shows no number.
- `main_sound_unload_scenario_mus` releases a slot's music record and SMD
  without clearing `g_main_sound_music.state` when that slot is the playing
  one. The shop, formation and both save screens unload their own music this
  way, so the next `main_sound_stop_current_music` hands the released record
  to `SuzukiDeallocateMUSChannels`.
- `battle_map_stop_texture_animation` clears the active byte of slot `handle`,
  but `battle_map_start_texture_animation` returns slot + 1: it stops the slot
  after the handle's own, and handle 8 clears the byte after the eight slots,
  the first of `g_battle_gfx_cycled_deployed_palettes`.
- `battle_gfx_append_unit_graphics_load_descriptor` writes its record before it
  tests the count, which stops at 16: a seventeenth append lands one past the
  16-entry queue, on the head of `g_main_zodiac_primitives`.
- `battle_effect_remove_secondary`: removing a record from the middle of the
  active chain leaves the record before it with `next_slot_id` naming the freed
  record. When that one is removed while a newer effect is still active, it
  patches the freed record's link (by then a free-chain link) and its real
  successor keeps naming it as the previous record.
- `main_heap_alloc` and `main_heap_alloc_smd` tag a new block one above the
  highest tag in the cells before it, so it can share a tag with the block
  right after it (allocate 62 cells, then 2; free the first; allocate 10, then
  52: the last two blocks are both tag 2). `main_heap_free` clears cells while
  the tag repeats, so freeing the 52-cell block frees the 2-cell one too,
  whose own free then returns 0. No retail sequence is known to do this.
  `main_heap_reserve_at` picks an unused tag.
- `main_unit_calculate_entd_equipment`: the random body-armor roll tests
  `BATTLE_SUPPORT_SET_1_EQUIP_SHIELD` before asking for `ITEM_TYPE_ARMOR`;
  Equip Armor is tested only for the helmet.
- `battle_unit_store_coordinate_mount_animation_data` and
  `battle_unit_restore_facing_after_attack`: for a mounted pair the second
  `battle_unit_set_tile_position` call names the unit's own battle record again
  (the first function finds its "mount" by the unit's own `unit_id`), so the
  partner's battle stats take no tile or facing here. The second function also
  writes the partner record's facing with no null test.
- `battle_gfx_release_reserved_vram_slot` tests owner bit 0x80
  (`BATTLE_GFX_VRAM_SLOT_OWNER_ATTACK_IMAGE`), which the EVTCHR (0xfe) and free
  (0xff) markers also carry, so the event instruction releases a saved EVTCHR
  slot too.
- `battle_effect_draw_projectile_model`, the Reflect shield: the depth is
  assigned from each corner in turn where the other models sum them, so only
  the last corner counts, and every triangle is linked at ordering-table entry
  0 whatever its depth.
- `battle_map_load_data`: the fourth selection pass does not exclude filler
  rows (type 0x31; 1,533 in the retail GNS files, all with variable 0x22, state
  1 and comparison equal). One whose comparison holds indexes entry 49 of the
  49-entry `g_battle_map_loaded_gns_records` and
  `g_battle_map_pending_gns_records`: it reads
  `g_battle_map_saved_light_matrix` and can overwrite the first 20 bytes of
  `g_battle_map_local_light_matrix`.
- `main_gfx_build_zodiac_primitives` and `card_gfx_build_zodiac_primitives`:
  the round-to-nearest step of the sprite scale adds 1, not `ONE`, to the 4.12
  product before dividing, so it only rounds up a remainder of `ONE` - 1.
- `battle_state_handle_open_sp2_files_state` frees its 0x8000-byte buffer when
  `battle_gfx_open_sp2_file` returns 0, which means the read has started, and
  keeps it on 1 (loader busy). The next frame allocates again and has its
  second request refused, so the file arrives in the kept block only because
  the heap hands the freed pages back.
- `battle_effect_set_secondary` (fall dust, splash, level up, venom trap,
  zodiac poof, death smoke, teleport) builds its init record in a local of
  which `battle_effect_build_secondary_init` sets five fields;
  `battle_effect_init_secondary` copies the rest of the two ten-byte blocks and
  `used_weapon_id` as the stack left them.
- `battle_gfx_draw_unit_shadow` sets bit 0 of `shadow_dirty` before testing it,
  so the shadow quad is recalculated on every call and the dirty bit the
  movement code sets gates nothing.
- Tests that cannot succeed: `battle_state_sync_frame` zeroes a
  `g_main_system_min_vsync_interval` of 1 under `wait < g_main_system_min_vsync_interval` with `wait` at least 2;
  `battle_state_run_battle_setup` loops until
  `battle_state_update_deployment_controller_input` returns -1, and it always
  returns 1; the first `switch` of `battle_move_apply_unit_step_velocity`
  repeats the layer test that the statement before it has just made false;
  `battle_turn_get_status_flags` cannot report Sleep, whose
  `g_main_status_effect_data` record carries the freeze-CT flag tested first;
  `battle_unit_set_animation_based_on_mounted_state` tests a depth masked to a
  byte for `< 0` and a nonzero animation for 0; `battle_menu_get_thread_activity`
  tests `i != 3` inside its scan of threads 4-9;
  `battle_camera_rotate_when_unit_tile_not_visible` adjusts a negative
  `direction` after masking it to 0-0xfff; selector 15 of
  `battle_map_set_tile_data_value` retries `battle_map_tile_data_15_stub`,
  which returns 0 at once, up to 1000 times before load exception 0x80.
  `battle_gfx_run_unit_seq_script`, `battle_gfx_run_wep_eff_seq_script` and
  `battle_gfx_load_trap_and_unit_frame_parts` test sequence and frame pointers
  for -1, which the unpackers replace with the start of the data, and the last
  tests a three-bit part count plus one for `> 8`. The weapon interpreter's own
  bounds (`sequence_id > 0x100`, `frame > 0x200`) admit the index one past each
  table.

## Calls that disagree with the callee

Where callers pass arguments a callee never reads, the callee declares them as
`unused_` parameters and the calls are plain. The calls below cannot be
written that way.

- `StCdInterrupt`: emulated header/payload copies pass a fourth completion argument to the three-argument `mem2mem` helper, which ignores it.
- `g_battle_thread_call_target` is the main-stack dispatch slot for callees
  with different signatures; assignments erase their function types.
- `world_card_build_save_slot_description` uses the job-name pointer that the
  void `world_text_bind_section_data` leaves in `$v0`; a native build must
  return it.
- `src/world/world_menu_resize_parent_entry_to_digits.c`: calls
  `world_text_count_decimal_digits` with no argument; the callee reads a stale
  `$a0`. Nothing calls the function itself.
- `src/battle/battle_move_animate_fall_to_target_tile.c` passes a coordinate
  buffer to the one-argument
  `battle_gfx_calculate_screen_z_from_misc_screen_data`, as it does to
  `battle_gfx_calculate_screen_z_with_caller_data`;
  `battle_move_animate_jump_arc_to_own_tile.c` does not.
- `g_main_smd_modulator_waveforms` entries 8-15 call the void
  `main_smd_modulator_deactivate`; `main_smd_update_modulators` adds the
  leftover `$v0` (the cleared modulator flags) to the channel output.
- `main_sound_update_tunes` passes `current_music` to `SuzukiGetMusicPlaying`
  even when no scenario music is loaded; the null record reads the status
  halfword at address `0x10`.
- `src/world/world_menu_animate_window_quad_crop.c` passes an uninitialized
  source (a stale `$s5`) to `world_menu_zoom_cursor_frame`, which never reads it.
- `battle_map_init_units_sprites_event_and_music` and
  `battle_map_step_init_sequence` call `main_sound_stop_sfx` without its sound
  id: step 0 passes `map_id`, still in `$a0`; step 11 passes whatever
  `main_sound_unload_current_scenario_music` left there.
- `g_equip_menu_list_row_callbacks` holds `s32 (*)(s32)` entries, but slots 2
  and 3 are `equip_item_build_type_icon_source` and
  `equip_menu_get_entry_item_icon_source`, which return record pointers.
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
- `battle_script_get_misc_id`: for the active-turn selector 0x69 with no unit
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
- `jobstts_menu_run_display_script_with_mode` takes the input word through its `void*`
  second parameter.
- `src/event/bunit_entrypoint.c` passes a second argument, 0, to the
  one-parameter `bunit_gfx_draw_falloff_shaded_rows`; its other caller passes
  one.
- `battle_menu_init_system_function` returns `$v0` as the void
  `battle_text_determine_spell_quote` left it.
- `battle_effect_spawn_emitter_particles` has an `s32` result it never sets
  and no caller reads.
- `battle_thread_resolve_id`, `battle_thread_resolve_id_after_current`,
  `world_thread_resolve_id` and `world_thread_resolve_id_after_current` return
  the leftover `$v0` of the void `battle_thread_exit_current` or
  `world_thread_exit_current` when no thread slot is free (the WORLD source
  falls off its end). The caller has exited by then, but the scheduler
  resumes slot 0 without testing it, so a main-loop caller gets that value
  on the next frame.
- `battle_ai_calculate_clockticks_until_death_counter_expires` is defined
  with three parameters and both callers pass one; the other two are scratch
  it assigns before reading.
- `battle_map_calculate_slope_height` masks its level to a byte; two callers
  pass the 16-bit `action_target_elevation` unmasked.
- WLDCORE passes its 8-byte records by value:
  `wldcore_window_render_bounds16_t` to the three render-record builders and
  `wldcore_point32_t` to `wldcore_window_build_yes_no_panel`.
- `battle_script_filter_unit_id_by_mode` and
  `world_script_filter_unit_id_by_mode` read `in_id` as a halfword; most
  callers pass the address of an `int` loop counter (BATTLE through a
  `(u16*)` cast), so the id is that counter's low half.
- `battle_script_process_pending_requests` assigns the results of
  `battle_map_command_get_3d_object_state` and
  `battle_map_command_get_texture_animation_active`, which are defined `void`
  and have no prototype in `battle.h`; the value is the `$v0` that
  `battle_map_dispatch_map_data_command` left.
- `battle_move_calculate_walkto_pathing` has no return statement;
  `battle_move_start_unit_walk_to` uses the path that `battle_move_calculate_pathing` leaves in
  `$v0`, which a native build must return.
- `battle_map_load_gns_and_move_find_items` has no return statement for a map
  whose GNS sector is 0 (map ids 120-124, 126 and 127 in the retail table);
  `battle_map_load_data` compares the leftover `$v0` of the BIOS `bzero` with
  `g_battle_map_gns_records`. A native build must return a value there. When
  the result is not the table (also when the read could not start),
  `battle_map_load_data` returns 0 with its 0x20000-byte load buffer still
  allocated.
- `src/battle/battle_reaction_try_distribute.c` passes the unit twice to the one-parameter
  `battle_reaction_fails_brave_roll` through a function-pointer cast; the second argument is ignored.
- `src/battle/battle_formula_calculate_critical_hit.c` calls the argument-less
  `battle_formula_calculate_knockback` through a cast with XA's address in `$a0`; the callee ignores it.
- `src/battle/battle_formula_apply_ability_element.c` calls the argument-less
  `battle_formula_nullify_action` through a cast with `mount_info` in `$a0`; the callee ignores it.
- `battle_gfx_set_thrown_item_graphic_palette` calls
  `main_item_get_data_pointer` with no argument, through
  `get_item_data_pointer`, a second name `target/main.yaml` gives 0x8005a884;
  the callee takes the item id still in `$a0`.
- `g_battle_effect_secondary_handlers` entries 0x07, 0x0a, 0x0b, 0x0e and 0x10
  are empty `void` functions, although `battle_effect_update_secondary_effects`
  ends an effect on a handler result of 0. No record of
  `g_battle_effect_secondary_animations` names them.
- `jobstts_ability_is_non_action_learned` passes one word (0x801f0050, named
  both `g_jobstts_selected_ability_data` and
  `g_jobstts_selected_ability_secondary_data`) for both outputs of
  `main_ability_calculate_pointers_and_type`, so the ability data pointer is
  overwritten by the secondary data pointer.
- Calls that omit arguments the callee reads take them from whatever the
  registers hold; a native build must pass them explicitly:
  - the three `battle_camera_get_input_direction` callers pass only `mode`;
    `input` is the leftover `$a1` when `g_battle_cursor_input_mode` is not 0/1;
  - `battle_camera_handle_tilt_input` passes nothing to
    `battle_camera_toggle_tilt`, whose tilt-target-1 path plays its sound
    with the leftover `$a1`;
  - the five `battle_effect_init_data` callers pass nothing, so the states
    it does not handle return the leftover `$a0`;
  - `bunit_gfx_build_item_graphic_descriptor` and
    `equip_gfx_build_item_graphic_descriptor` call
    `battle_gfx_set_item_icon_sprite` without the item id (`$a1`);
  - `battle_status_resolve_unit_changes_in_preview` (referenced nowhere on the disc) calls
    `battle_status_resolve_unit_changes` without `unit_id`/`removal_only`;
  - `world_script_is_deployment_running` calls `world_script_run_event_frame`
    without `ot`/`buttons`.
- These calls rely on the caller's own incoming registers instead, which
  hold the right values: `equip_text_start_character_thread_if_idle` (thread id in `$a0`)
  for `battle_thread_is_running`, `battle_unit_start_post_attack_animation_display`
  (both arguments) for `battle_unit_set_target_animation_from_attack_type`, and
  the EQUIP selection wrappers (`input_mask` in `$a2`) for
  `equip_menu_update_wrapped_horizontal_selection` and its vertical twin.

## Declaration leads

- `include/fft/menu.h`: `world_menu_entry_t.window_x` is `u16`, but
  `src/world/world_menu_open_entry_window.c` needs it signed.
- The unit status record (`battle_unit_status_record_t`, `include/fft/menu.h`)
  is laid out again by the heads of `equip_unit_data_t`, `bunit_unit_data_t`
  and `world_formation_unit_t`, by `world_item_stat_summary_t` and by
  HELPMENU's raw `g_helpmenu_active_banner`.
- `equip_gfx_context_t` (`include/fft/event_equip.h`), `jobstts_gfx_context_t`,
  `bunit_gfx_context_t` and `world_gfx_packet_buffer_t` share one 25-pointer
  pool layout (BUNIT and WORLD match through `0xec`); their pool names disagree.
- `g_wldcore_zodiac_start_dates[12][2]` (`include/fft/wldcore.h`) and OPEN's
  `g_open_birthday_zodiac_start_dates` are the same `{month, day}` table; the
  OPEN copy stays a flat `[24]` because the `[12][2]` spelling changes its
  reader's code.
- The movement step byte is one bit-field record (`battle_move_step_t`) in
  three places: the path elements (`battle_unit_misc_data_t.movement_path`),
  `battle_unit_misc_data_t.current_path_step` and the working global
  `g_battle_move_step_value`.
- `g_world_gfx_full_texture_window` is a `RECT`, but
  `src/world/world_gfx_build_zodiac_primitives.c` reads its first 4 bytes
  as a screen point.
- `g_main_item_location_flags` (`0x80059414`): only the first 64 bytes (512
  Move-Find bits) are proven; the next 64-byte block is saved with it but has no
  known meaning.
- `world_menu_init_quad_from_record` and `world_gfx_init_image_loading` take
  `u16*`/`POLY_FT4*` but handle a `RECT`, an image record and either a
  `POLY_FT4` or a `SPRT` (code `0x64`, whose `w`/`h` overlap `x1`/`y1`);
  `world_main_menu_text_window_thread` passes a `SPRT` through a cast. A
  typed version needs a primitive union or separate SPRT/POLY_FT4 paths.
- `void*` parameters whose callers pass another view of the data:
  `battle_gfx_set_draw_mode_from_rect` (a window record for its `mode0`
  `DR_MODE`), `battle_map_store_selected_tile_coordinates` (an `s16[3]` as an
  `SVECTOR`; only three halfwords are written), `open_gfx_load_opntex_into_frame_buffer`
  (OPNTEX bytes read as words), `world_menu_build_sprite_page` and its BATTLE
  twin (a `RECT` as an image location), `world_menu_init_and_load_scrollable_list`
  (menu scripts as a list record), `battle_target_build_directional_attack_panels` (a
  `battle_ai_command_action_t`, copied into a file-local duplicate of it) and
  `world_menu_run_script_with_palette_mode` (display scripts declared as
  `u8[]`, `s16[]` or `world_menu_window_command_t`).
- `battle_unit_misc_data_t` (`include/fft/battle.h`) repeats `battle_strike_work_t` field by field
  at `0x18c`..`0x1b3` (the strike work's last four bytes are the misc record's `action_rewards`);
  `battle_action_start_strike` and `battle_action_resolve_ability_strike` reach it through a cast
  of `&misc->action_18c`, and `battle_action_apply_strike_results` through another.
- `g_wldcore_sound_quad_command_bytes` (`0x8009ec18`) starts one row before
  the sound rows, which begin at `0x8009ec1c` for sound id 1: its row 0 is the
  eighth word of `g_wldcore_tutorial_event_ids`, whose eight entries
  `wldcore_list_handle_battle_tutorials_input` indexes.
- `g_battle_effect_ending_secondary_function_id` (`0x801b8530`) is
  `g_battle_effect_secondary_animations[BATTLE_EFFECT_SECONDARY_ANIMATION_ENDING].function_id`
  under a second name; the table's own symbol covers the 21 records before
  it.
- `0x80044128` has two names in `target/main.yaml`: the function
  `main_sound_stop_current_weather_sfx` and the data row
  `main_sound_pause_tracked_sfx`, the name BATTLE calls and `include/fft/main.h`
  declares.
- `g_open_file_openbk_end_sector_offsets` (`0x80073f5c`) is
  `g_open_file_openbk_start_sector_offsets + 1`: eight sector offsets of
  OPNBK.BIN under two names, each image ending where the next starts.
- `g_open_prologue_crossfade_progress` (`0x800854b5`) is
  `g_open_prologue_crossfade_row_flags[1]`;
  `open_prologue_prepare_crossfade` clears it through the array.

## Duplicated code

Twins stay separate functions because each lives in its own module or
translation unit. Share their types and constants through headers.

- EFFECT: 32 groups of byte-identical routines across 110 native files. E005's
  first routine appears in 17 overlays, and E450/E480 are identical.
  E336/E461, E230/E481 and E015/E047 differ only in immediates.
- BATTLE/WORLD pairs: scenario conditionals
  (`battle_script_run_scenario_conditions.c` /
  `world_script_run_scenario_conditions.c`), action-slot restrictions, the
  portrait-flip toggle (instruction `0x37`), staged status, selected-tile data,
  and the event interpreters.
- `src/wldcore/wldcore_menu_push_fact_detail_level.c` and
  `src/wldcore/wldcore_menu_push_story_event_text_level.c` are one routine at
  menu levels `0x2f` and `0x1e`; the second reserves 8 unused stack bytes.
- `battle_map_blend_ambient_light_color.c` and
  `battle_map_blend_darkness_color.c` are one 1,032-byte function over
  different records.
- `world_gfx_init_scaled_draw_area_packets{,_2,_3,_4}.c` differ only in the
  eight bytes they install as the second `RECT`.
- Routines compiled twice into one module, identical apart from the addresses
  they jump to, call and load: `battle_script_run_next_event` and
  `battle_script_run_next_event_unused` (and the WORLD pair);
  `battle_unit_add_event_offset_by_misc_id_unused` and
  `battle_unit_add_event_offset_by_misc_id`;
  `battle_move_set_float_step_delta_center_to_edge` and
  `battle_move_set_float_step_delta_edge_to_center`, which repeat the
  `battle_move_set_unit_step_delta_*` pair;
  `battle_text_advance_cursor_with_backreferences` and
  `battle_menu_text_advance_cursor_with_backreferences` (and the WORLD pair);
  `world_menu_set_cursor_position` and `world_menu_init_cursor_position`;
  WORLD's `world_option_gfx_*` and `world_screen_gfx_*` scaled-draw-area
  routines and its two thin-window-frame threads. The two copies named unused
  have no caller.
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
  `src/event/bunit_menu_script_draw_plain_number.c`.
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
- `src/wldcore/wldcore_input_check_repeating_directional.c`: the arms test
  UP, DOWN, RIGHT, LEFT, R1, L1, but the counters sit in UP, DOWN, LEFT, RIGHT,
  R1, L1 address order. The early-out clears only the four direction counters.
- `src/wldcore/wldcore_proposition_select_report_speaker.c`: with more than one
  unit sent and a speaker mode other than 1-4, the returned `selected` is never set
  (every retail row uses mode 4, the lowest Faith).
- `src/wldcore/wldcore_map_roll_random_encounter.c`: encounter masks count from
  the high bit, so the chosen ENTD is `entd_ids[7 - bit]`.
- `wldcore_map_step_route_travel` stores the 0-based arrival location in the map
  cursor level's one-based `hovered_location` (`location_transition.location_id`,
  +0x0c); the cursor's next snap probe overwrites it before the label reads it. That
  level's `pending_mode` is only ever 0, so its unblock-and-leave branch never runs.
- `src/battle/battle_action_finalize_draw_out_katana_result.c`: when the
  katana breaks, the strike work's `can_earn_experience` is copied onto itself.
- `src/battle/battle_menu_run_icon_selection_loop.c` and its WORLD twin: the
  loop clears both records' `+0x78` words when they hold 0 and 2, but only the
  record builders write that word (always 0), so the reset never fires.
- `src/wldcore/wldcore_menu_step_treasure_detail_level.c` and its unexplored-land
  twin: phase 1 waits on the render record named by `+0x08` (`saved_record_index`),
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
- `src/event/equip_menu_load_images_and_reset_lists.c` and `bunit_gfx_init_vram_and_start_fade.c`: store 16-colour CLUTs into the 4-entry `buf1`; they run on into `buf2`, which the frame places directly after it (the source reads as two arrays, the stack holds one).
- EFFECT scripts never run seven state handlers: E000's script calls only `effect_e000_noop` (opcode 07) and E314's names no file function, so native function ids 1 and 90 are bound by nothing; E134/E242 `update_ring_stack_mesh_state`, E454/E455 `map_set_3d_objects_to_state_2_state` and E456 `map_step_freeze_state` sit in callback slots (script opcode 06) that no keyframe selector fires. E457.BIN, which has no code, binds native function 88 (0x801c2578, E259's `effect_e259_update_summon_mesh_state`; data in its own image) to slot 0, and none of its keyframes names a slot.
- `src/battle/battle_map_draw_mesh_and_weather.c`: when map command 0x96 (E458) ends, `battle_map_run_weather_command(0x8b)` restores a non-snow rain or storm mid-draw; the mode-0x55 block then re-links `ft3` drops the 0x96 block already linked at OT+2, an ordering-table cycle that holds `DrawSync` until its 240-VBlank timeout.
- `src/world/world_card_run_menu_screen.c`: its 0xCC60-byte stack frame of packet pools reaches below 0x801ff000 into the top cells of MAIN's game heap (0x801df000-0x801ff000); it is safe only while those cells are free.
- `src/world/world_tutorial_command_end.c`: two paths fall off the end of the s32 handler and return a stale nonzero `$v0` (the fade's result, or the 1 just stored), so the formation loop keeps running.
- `src/world/world_menu_draw_thread_status_indicators.c` and its BUNIT twin (the ATTACK, DEBUGCHR, EQUIP and REQUIRE copies have no caller): while a unit-status banner thread (8 or 7) is being stopped its first parameter is 0, and the panel check reads that "display record" at RAM address 0.
- `src/open/open_script_draw_text_records.c`: passes OT entries by value (`otag[15]`, `otag[8]`) to `AddPrim`; after `ClearOTagR` each holds the physical address of the entry below it, so the credits fade, bands and glyphs link into layers 14 and 7 through the KUSEG mirror.
- `src/wldcore/wldcore_gfx_step_dissolve_image_upload.c`: the step-0 call (start the dissolve) has no return value of its own; `$v0` keeps its clearing cursor, the address one byte below the band-progress table. Its three callers ignore that result.
- `src/world/world_menu_build_icon_record.c`: for the Bar's send-unit list (header 0x10) the arrow x reads the "height" of the record at the panel's value pointer, `&g_wldcore_active_menu_value` + 6, which is the upper half of `g_wldcore_proposition_dispatch_days` (0).
- `src/battle/battle_unit_generate_treasure.c`: outside action execution (AI simulation, preview) it returns with no value, so `$v0` still holds `g_battle_action_state` and the crystal pickup result names item 1 or 2 as the treasure.
- `src/open/open_title_step_new_game_start.c`: no retail code sets step 5, which pops New Game and opens the Music Test (`open_menu_start_music_test_controller`, controller slots 7 and 8); only a poke of the step word reaches it. Community sources say the Japanese release opened it for the name "BGM kikitai" (in kana), a check this release does not have.
- WORLD's copy of the event interpreter never runs: `world_script_execute_event` is reached only through `world_script_start_current_event_thread`, whose callers `world_menu_start_entry_thread` (no caller) and `world_menu_dispatch_system_function` (reached only from `world_menu_init_system_function`, below) never run, and no table holds their addresses. It calls BATTLE routines (`battle_map_freeze`, `battle_gfx_load_unit_graphics_by_battle_id` and others), since WORLD links against BATTLE's addresses for the code it copies: `world_camera_wait_until_idle`, for one, calls `battle_camera_is_active` at 0x8008e17c, which with WORLD loaded is inside WLDCORE's `wldcore_map_find_best_route_path`. Its camera threads (`world_camera_thread`, `world_camera_fusion_thread`) never run either.
- WORLD's copy of the battle menus is dead code: nothing in any module calls or names `world_menu_init_system_function` (0x800f5230) or `world_menu_start_mini_menu_display_thread` (0x800f0e48), so modes 1 (AT list) and 2 (crystal pickup panel) of `world_option_run_menu` never run; the world map's Options row runs mode 0.
- `src/battle/battle_script_get_misc_id.c`, `src/world/world_script_get_misc_id.c`: during
  Game Over event `0x194` every unit lookup resolves to Ramza, but no retail
  code, scenario chain, `BTLEVT.BIN` condition, event or world script starts
  `0x194`; the engine plays the byte-identical script at `0x190` without the
  redirect.
- Abilities no skillset lists: 0x16f (Attack), Reflect (0x1b8), Non-charge,
  Teleport 2, Move Underwater and Silent Walk. Four ids with no name text are
  played as Altima's teleport effects (0x28, 0xb8, 0xdb, 0xdc), and 0x200 is
  the follow-up strike that resolves a knockback.
- Skillset rows 0x86-0x8d, 0xa7 and 0xa8 have no name text but hold
  abilities; 0xa7 is the row the AI uses for a unit under Blood Suck, and
  0xa9, which it pairs with ability 0x16f, is empty.
- The ability names "Move in Water" (0x1f6) and "Walk on Water" (0x1f7) are
  the reverse of what the abilities do: 0x1f6 keeps the unit on the surface
  and 0x1f7 one level below it (`battle_unit_get_effective_height`,
  `battle_move_store_unit_movement_to_scratchpad`).
- Selling in the fur shop adds the sold copies to the poached stock
  (`world_shop_run_fur_sell_confirm_step`), so they can be bought back there.
- `world_shop_run_fur_buy_quantity_menu` divides the gil on hand by
  `price >> 1` with no lower bound, while the sell side
  (`world_item_get_half_price`) keeps half price at 1 or more.
- `battle_move_start_teleport_step` builds a secondary-effect record in a
  local and never uses it.
- A change to a map CLUT row reaches VRAM on the second call of
  `battle_map_upload_clut_rows`: one call loads the 256x14 image to y 0x1ee,
  the next moves it to y 0x1e0. Rows 8 and 9 have state and are uploaded, but
  no clut word in the code addresses them.
- Rolls one off their round number: `wldcore_map_roll_random_encounter` tests
  `chance >= pick` over 0..99, so the retail chance byte 30 gives 31 in 100;
  the preferred-job promotion in `wldcore_proposition_determine_success`
  passes on a roll below 0x3D, 61 in 100; `battle_formula_apply_status_to_action`
  divides the displayed accuracy of separate statuses by four and keeps each
  status on a roll below 24.
- `battle_unit_apply_level_up_down_ability` stores the new level before
  calling `main_unit_apply_level_growth`, so an ability level-up divides each
  raw stat by growth + new level; an EXP level-up (`main_unit_check_level_up`)
  and the generated-unit replay in `main_unit_calculate_actual_stats` divide
  by growth + old level.
- `battle_unit_find_war_trophies_and_bonus_money` computes
  `level_bonus_money` ((highest enemy level - lowest party level) * 100, 0
  once a player unit has fallen) and nothing reads it; REQUIRE pays
  `level_sum_money` and `bonus_money`.
- `src/battle/battle_ai_load_ability_entry.c`: with Throw Item the AI gives an
  item a range of the unit's Move; `battle_target_set_panels_for_action` uses
  4, and gives a Mime the same.
- `battle_process_map_gns_record`: command rows 0x85, 0x86, 0x88, 0x89, 0x8a
  and 0x8b skip the map-state comparison and are applied in file order, so the
  last of a kind wins: MAP017.GNS has one set-outdoor row followed by six
  set-indoor rows and ends indoors. Row types 0x87 (49 rows in 14 maps) and
  0x8a (125 rows) have no case in `battle_map_dispatch_gns_resource`, and no
  retail GNS holds a 0x83, 0x84 or 0x8b row, the three that take arguments.
- Terrain data the code does not expect: five tiles hold a slope byte outside
  `map_slope_type_e` (0x10 in MAP017.10, MAP043.9 and MAP078.9, 0x46 in
  MAP036.46, 0x01 in MAP045.7), and the terrain chunks of 37 maps arrive with
  mark bits already set in tile byte 5, which
  `battle_map_copy_size_and_tile_data` copies as they are. Tile flag 0x80
  (`cannot_stop`), which `battle_move_set_reachable_tile_flags` tests, is set
  in no terrain chunk, and tile byte 1 is 0 in all of them.
- `g_battle_map_file_table` is the string `C:\FFTIMG\MAP\GNS\`; every map
  file load passes it as the first argument, which
  `battle_map_start_file_load` ignores.
- The Brave Story "Fact" row never appears:
  `EVENT_SCRIPT_VAR_FACTS_ENABLED` (0x92) gates it
  (`wldcore_menu_build_brave_story_entries`), no code sets it, and the one
  event script that writes it, 0x075, stores 0 while setting facts
  0x1a4-0x1aa. The 24 fact names in WORLD.LZW are empty strings.
- `g_wldcore_treasure_sound_novel_ids` is 0 for every treasure but 31
  (Germonik Scriptures, novel 5), so `wldcore_menu_step_treasure_detail_level`
  opens no reader for the four book treasures 17-20.
- World-map script opcodes 0x10 and 0x11 pass on a date on or after and on or
  before their operand (`wldcore_script_check_date_on_or_after`,
  `_on_or_before`); scenario condition types 16 and 17 are the reverse
  (`EVENT_SCENARIO_CONDITION_DATE_ON_OR_BEFORE`, `_ON_OR_AFTER`), although the
  war-funds and casualty tests beside them share their numbers.
- The Music Test names and comments (OPEN.LZW text sections 24 and 25, text
  ids 0xC000 and 0xC800 + entry) are not translated: each string is the
  Japanese release's glyph codes around Latin fragments such as the composer
  tags (`open_menu_update_music_test` shows them).
- SCUS_942.21 0x80028f94-0x80028fe7, between `g_main_smd_modulator_waveforms`
  and `g_main_smd_key_octaves`, is referenced by no instruction or pointer in
  any executable: a five-byte SMD sequence (`B0 91 7F 99 90`), ten words whose
  last nine are the LIBSPU reverb work-area sizes of modes 1-9, and a
  0x23-byte `feds` sound bank with id 0x7000.
- Ability data the code and the tables disagree on:
  `battle_target_mark_ability_area` passes range-record `flags_2`
  bit 0x20 (`ABILITY_SECONDARY_FLAG_2_TOP_DOWN`) to
  `battle_target_apply_vertical_tolerance` as `single_layer`, and none of the
  368 records sets it; the low nibble of
  `ability_data_t.type_flags` holds the ability type plus one (0 for ids 0,
  0x1fe and 0x1ff) and nothing reads it, because
  `main_ability_calculate_pointers_and_type` derives the type from the id.
- `main_status_init_check_data` builds eleven status check sets; sets 2 and 7
  (`MAIN_STATUS_CHECK_SET_UNREAD_2`, `_UNREAD_7`) are read by nothing.
- WORLD's copies of BATTLE's action-menu and tile-info routines
  (`world_menu_build_ability_list`, `world_menu_build_skillset_entries`,
  `world_menu_selected_tile_info_display_thread` and nine more) keep BATTLE's
  absolute data addresses 0x80173cbc, 0x8017403c, 0x80174048 and 0x8017404c.
  In WORLD's image those lie inside the text file embedded at 0x8016ee68, so
  the routines would read and write text bytes there.
- `g_main_terrain_geomancy_abilities` gives Gusty Wind for Sky and Kamaitachi
  for Darkness, Cross section and the unnamed surfaces 0x2f-0x3e; the terrain
  help text says "None" for Sky, Darkness and Cross section.
- `battle_camera_update_rotation` chains a held R1 through
  `battle_camera_set_left_turn_destination`, as it does L1; the two helpers differ only in
  which way they move a destination that is a multiple of 0x400.
- `main_unit_copy_job_data` adds the immortal and formation immunity sets
  only to a unit that is both immortal and flagged save- or load-formation;
  the per-flag tests inside the loop are then always true.
  In the retail ENTD files 46 units are both. The other 119 immortal units
  resist, beyond their job's immunities, only the formation set, which
  `battle_status_modify_inflictions` applies to every immortal target, so
  they can still be Confused, put to Sleep or given Don't Act.
- `battle_formula_apply_status_to_action`: the
  `BATTLE_ACTION_STATE_PREVIEW` test in the separate-status loop cannot pass;
  any state but EXECUTE was switched to all-or-nothing above it.
- `main_gfx_build_now_loading` loads the NOW LOADING CLUT to VRAM (0x70,
  0x1e2), clut word 0x7887; in battle that is
  `MAP_CLUT(MAP_CLUT_ROW_SPRITES, 7)`, the status bubble CLUT.
- The effect interpreter indexes fixed tables with wider fields and no check:
  the opcode is masked to nine bits over the 120 entries of
  `g_battle_effect_instructions` (0x2e-0x77 are the error report, a `void`
  function in a table of `s16` handlers: `battle_effect_run_block` would test
  its leftover `$v0` and the script position would not move);
  `g_battle_effect_particle_render_handlers` has four entries for
  `(kind_flags >> 12) & 0xf`, so `EFFECT_WORK_KIND_HEAP_BLOCK` would call
  through `g_battle_effect_camera_rotation_mode`; opcode 06 takes a six-bit
  callback slot over the five words of `g_battle_effect_callback_slots`, and a
  keyframe action names slot 1-7 over the record's four (5 writes
  `target_index`). Retail files use opcodes below 0x2e, kinds 0 and
  2 and slots 1-4.
- Special unit name 0x44, "Rofokare", is given to no ENTD unit; every other
  named entry of the 256 is.
- `battle_heap_alloc_block` has no out-of-memory exit: when no free block is large enough it
  walks the circular free list forever.
- WORLD runs threads in slots 1-16: `world_thread_yield` wraps at 17, and `world_thread_resolve_id`
  and `world_thread_find_running_by_task` scan to 16 (BATTLE stops at 15). The array has room for
  the 17th slot (`g_world_gfx_tpage7_vram_allocation_grid` starts right after it), but
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
- `src/main/main_ability_store_skillset_abilities.c`: the support filter takes ids below `0x1e7`, so
  Move+1 (`0x1e6`) counts as a support ability and not a movement one. Every caller asks for action
  abilities or for all of them, so nothing shows it.
- `src/battle/battle_menu_display_projected_action_effect.c`: the 0x100 bit that
  `main_status_find_action_highest_order_effect` adds for a status flagged
  `STATUS_EFFECT_FLAG_2_SCRIPT_FILTER_EXCLUDED` changes nothing. The preview tests 0x80 (a removal) first, so
  its 0x180 test, which would pick marker image 0x1e, never passes.
- `src/battle/battle_unit_set_animation_based_on_status.c` picks animation 0x21 for bit 0x01 of the
  status mirror `status_flags_1_4`, but no status sets that bit
  (`g_battle_misc_status_mask_by_handler_index`), so the animation never plays.
- `src/battle/battle_ai_add_usable_skillset_abilities.c`: for a Jump skillset with no learned Level Jump,
  the bare Jump entry is written past the list's count, so the next entry (the next skillset's or Frog
  Attack) overwrites it and the unit has no Jump entry.
- `src/battle/battle_ai_build_unit_ability_list.c`: the weapon kinds of both hands are ORed and only one
  kind at a time is handled (none, damage, absorb, heal), so a unit holding two kinds (a Blood Sword with
  another weapon under Two Swords) gets no target-team flag on its Attack entry.
- Half of MP: `battle_menu_get_unit_skillset_ability_data` lists the half rounded up and the menu greys
  an ability by it, but `battle_action_check_and_consume_mp` and the AI use the half rounded down. Seven
  abilities have odd costs (Zodiac 99; Toad 2, Gravi 2, Flare 2, Blind 2, Confuse 2 and Sleep 2 at 35),
  so a unit with exactly the lower half sees them greyed.
- `src/battle/battle_ai_handle_autobattle.c`: Protect allies falls back to Run like a rabbit when
  `battle_ai_select_protect_allies_action` returns -2, which it never does.
- `src/battle/battle_ai_evaluate_origins.c`: an ability without
  `BATTLE_AI_ABILITY_EXHAUSTIVE_ORIGIN_SCAN` multiplies its origin and target tile counts and tests
  the byte product against 10, but both arms set `ranked_origin_search`, so every such ability
  takes its origins best first (`battle_ai_take_best_origin`).
- `src/battle/battle_ai_is_unit_untargetable.c`: a Transparent enemy is left out of the AI's
  targets, except a rider whose mount is Transparent too.
- `battle_menu_run_status_screen_thread` hands a unit with mount info (a
  rider or a mount) to DEBUGCHR.OUT's `debugchr_editor_run_unit_thread`
  (`battle_menu_open_rider_status_screen`); any other unit gets BATTLE's own
  panel.
- `src/event/debugchr_editor_run_session.c`: the debug editor moves and clears the job, equipment
  and ability fields like the others, but `debugchr_editor_apply_values_to_battle_unit` writes back
  only HP, MP, CT, level, experience, Brave, Faith, the zodiac sign, Move, Speed and Jump, and the
  reload after each step (`debugchr_editor_load_selected_unit`) restores the unit's job, equipment
  and abilities.
- Functions that nothing references: no `jal` or `j` in any executable or overlay that can be resident with them targets them, and no word or `lui`/`addiu` pair there holds their address. Psy-Q library routines are left out, as are functions whose only callers are listed here.
  - BATTLE (118): `battle_action_activate_move_act`, `battle_action_load_last_used_ability`, `battle_action_store_8019387c_if_not_reacting`, `battle_camera_get_unit_sort_point_unbiased`, `battle_camera_save_offset_screen_coords`, `battle_camera_get_offset_screen_coords`, `battle_camera_start_tilt`, `battle_camera_restore_offset_screen_coords`, `battle_camera_set_offset_screen_coord_delta_from_value`, `battle_camera_set_offset_screen_coord_delta_from_values`, `battle_camera_set_offset_screen_coords`, `battle_camera_set_real_coord_delta_from_destination`, `battle_camera_set_real_coord_delta_from_values`, `battle_camera_set_rotation_delta_from_value`, `battle_camera_set_rotation_delta_from_values`, `battle_camera_set_zoom_delta_from_value`, `battle_camera_set_zoom_delta_from_values`, `battle_camera_start_zoom`, `battle_copy_misc_unit_screen_coords`, `battle_effect_add_2_to_halfword`, `battle_effect_add_4_to_halfword`, `battle_effect_add_6_to_halfword`, `battle_effect_add_8_to_halfword`, `battle_effect_get_list_node`, `battle_effect_get_prim_buffer_offset`, `battle_effect_get_screen_location_by_battle_id`, `battle_effect_init_fall_dust_secondary`, `battle_effect_init_single_target_trajectory`, `battle_effect_load_data`, `battle_effect_noop`, `battle_effect_noop_return_zero`, `battle_effect_replace_flags_0006`, `battle_effect_select_larger_magnitude_s16`, `battle_effect_start_basic_ability_animation`, `battle_formula_49_unused_with_extra_steps`, `battle_gfx_clear_screen_color_modulation`, `battle_gfx_copy_screen_color_modulation`, `battle_gfx_get_vram_slot_summary`, `battle_gfx_restore_screen_color_modulation`, `battle_gfx_set_sprite_part_flip`, `battle_gfx_set_thrown_item_graphic_palette_by_battle_id`, `battle_heap_print_stats`, `battle_map_adjust_background_color`, `battle_map_copy_background_color`, `battle_map_get_background_color`, `battle_map_get_weather`, `battle_map_restore_background_color`, `battle_map_set_ambient_light_color`, `battle_map_set_background_color`, `battle_map_stop_palette_animation`, `battle_map_transition_and_store_ambient_light_color`, `battle_map_transition_and_store_darkness_color`, `battle_map_update_all_palettes_from_color_data`, `battle_map_update_and_store_palette_colors`, `battle_map_update_palette_colors_by_index`, `battle_menu_clear_window_buffer_pair_by_thread`, `battle_menu_confirm_sound_delay_thread`, `battle_menu_display_text_entry_past_choice_marks`, `battle_menu_load_portrait_image`, `battle_menu_reset_unit_records_wrapper`, `battle_state_enter_ability_preview_help`, `battle_move_calculate_pathing_time_sliced`, `battle_script_run_next_event_unused`, `battle_script_set_current_otag_entry_and_update_input`, `battle_state_enter_effect_playback`, `battle_state_get_frame_measurement`, `battle_state_enter_effect_state`, `battle_state_enter_map_jumping_in`, `battle_state_enter_map_jumping_out`, `battle_status_resolve_unit_changes_in_preview`, `battle_text_measure`, `battle_text_set_substitution_values_7e_7f`, `battle_unit_add_event_offset_by_misc_id_unused`, `battle_unit_call_bow_hardcoding_by_battle_id`, `battle_unit_check_exists`, `battle_unit_check_party_member_exists`, `battle_unit_count_from_misc_data`, `battle_unit_erase_pending_party_removal`, `battle_unit_get_camera_facing_quadrant_by_misc_id`, `battle_unit_get_camera_facing_sixteenth_by_misc_id`, `battle_unit_get_screen_data_ptr_by_battle_id`, `battle_unit_init_party_member_battle_data`, `battle_unit_mount_by_misc_ids`, `battle_unit_restore_target_facings_after_attack`, `battle_unit_set_animation_from_facing_hint`, `battle_unit_set_movement_idle_animation_by_misc_id`, `battle_unit_set_target_anim_by_battle_id`, `battle_unit_store_camera_facing_animation_for_all`, `battle_unit_update_display_by_battle_id`, `battle_noop_8008de88` (and the same at 80134430, 80134a6c, 80137b90, 8013bd64, 8013bdc4, 8013c740, 8013caf4, 8013d624, 8013d62c, 8013da60, 8013da68, 8013da78, 8013da80, 8013f4a0, 8013f7ac, 8014268c, 80147918, 8014a824, 801a1328, 801a6010, 801a6018, 801a866c, 801a8e74, 801aad30, 801b0ff4), `battle_return_argument_80133494`, `battle_return_zero_8008e6d0` (and the same at 8013d1f0). `battle_camera_start_zoom` and `battle_camera_start_tilt` are bypassed (L2 and R2 go through `battle_camera_toggle_zoom` and `battle_camera_toggle_tilt`); `battle_formula_49_unused_with_extra_steps` is a second entry to formula 0x49's handler that the formula table does not name. The eight `battle_camera_set_*_delta_*` setters are the only code that starts the countdowns of `battle_camera_update_real_coord_animation` and its offset, zoom and rotation siblings, which the action-execute, event and map-change states call every frame and which never move anything. `battle_state_enter_effect_playback` and `battle_state_enter_effect_state` are the only code that stores `BATTLE_GAME_STATE_EFFECT`, so `battle_state_handle_effect_state` never runs.
  - WORLD (87): `world_copy_misc_unit_screen_coords`, `world_font_upload_glyph_image`, `world_formation_show_unit_message`, `world_gfx_append_gpu_primitive_to_secondary_otag`, `world_gfx_append_poly_f4_to_otag`, `world_menu_enable_height_display`, `world_gfx_flip_poly_ft4_uv_if_negative`, `world_menu_suppress_height_display`, `world_job_calculate_and_store_unlocks`, `world_map_get_weather`, `world_menu_clear_window_buffer_pair_by_thread`, `world_menu_confirm_sound_delay_thread`, `world_menu_disable_alternate_main_palette`, `world_menu_enable_alternate_main_palette`, `world_menu_get_crystal_pickup_selection`, `world_menu_get_selected_command_address`, `world_menu_icon_clamped_list_thread`, `world_menu_icon_strip_thread`, `world_menu_init_and_load_scrollable_list_with_scroll`, `world_menu_init_attack_resources_and_threads`, `world_menu_init_system_function`, `world_menu_load_portrait_image`, `world_menu_poll_small_load`, `world_menu_preview_ability_turn_at_list`, `world_menu_request_open_companion_data_file`, `world_menu_request_open_small`, `world_menu_end_at_list_unit_view`, `world_menu_reset_unit_record`, `world_menu_reset_unit_records_wrapper`, `world_menu_resize_parent_entry_to_digits`, `world_menu_restore_selection_for_selected_unit`, `world_menu_run_require_help`, `world_menu_run_thread_with_selected_index`, `world_menu_set_default_entry_0_text_values`, `world_menu_set_default_entry_1_text_values`, `world_menu_set_main_scroll_parameters`, `world_menu_start_entry_chain_threads`, `world_menu_start_entry_thread`, `world_menu_start_mini_menu_display_thread`, `world_menu_store_auto_battle_main_target_id`, `world_menu_take_and_clear_thread_result`, `world_menu_text_entry_thread`, `world_option_menu_thin_window_frame_thread`, `world_menu_update_menus_and_event_speed`, `world_noop`, `world_option_gfx_build_scaled_draw_area_packets`, `world_option_gfx_init_scaled_draw_area_packets`, `world_return_parameter_1`, `world_return_zero`, `world_screen_gfx_build_scaled_draw_area_packets`, `world_screen_gfx_init_scaled_draw_area_packets`, `world_screen_menu_thin_window_frame_thread`, `world_script_filter_controller_input`, `world_script_get_event_finish_operation`, `world_script_is_deployment_running`, `world_script_reset_event_state`, `world_script_run_next_event_unused`, `world_script_scale_music_volume`, `world_script_set_current_otag_entry_and_update_input`, `world_text_count_characters`, `world_text_print_fnt_value_and_next`, `world_text_set_substitution_values_7e_7f`, `world_text_start_thread_if_idle`, `world_thread_start_idle_5_to_8`, `world_thread_stop_after_empty_slots_5_to_8`, `world_option_thread_stop_after_yield`, `world_unit_update_facing_and_pending_animations`, `world_noop_800e8aa8` (and the same at 800e90e4, 800eba28, 800ef97c, 800ef9cc, 800f01f0, 800f05a4, 800f0f54, 800f0f5c, 800f1378, 800f1380, 800f1390, 800f1398, 800f2d18, 800f3024, 800f5f04, 800fad84, 80106138), `world_return_argument_800e7b54`, `world_return_zero_800f0b5c`. Most are its copies of BATTLE and overlay code; the quirks recorded above for `world_menu_resize_parent_entry_to_digits` and `world_menu_run_require_help` therefore never execute in WORLD.
  - WLDCORE (3): `wldcore_find_save_slot_entry_by_tag`, `wldcore_sound_novel_calculate_completion_percent`, `wldcore_text_get_padded_dimensions`.
  - OPEN (3): `open_card_wait_for_hardware_event`, `open_menu_init_render_record_fields`, `open_text_measure_menu_image`; the last is the only caller of `open_card_poll_hardware_event`.
  - MAIN (55): `main_sound_set_unread_value_800184e0`, `main_ability_get_aoe`, `main_ability_get_range`, `main_gfx_fade_in_orbonne_monastery`, `main_gfx_fade_out_orbonne_monastery`, `main_job_find_by_skillset`, `main_overlay_call_battle_return_zero`, `main_smd_dispatch_snapshot`, `main_smd_get_held_voices`, `main_smd_get_instrument_attr`, `main_smd_get_min_loop_count`, `main_smd_get_music_filename`, `main_smd_get_play_time`, `main_smd_init_channel_streams`, `main_smd_pause_music`, `main_smd_resume_music`, `main_smd_seed_random`, `main_smd_set_channel_mute_mask`, `main_smd_set_channel_select`, `main_smd_set_flags_all_channels`, `main_smd_set_stop_bar`, `main_sound_find_waveset`, `main_sound_flush_key_off`, `main_sound_flush_key_on`, `main_sound_free_wavesets`, `main_sound_get_current_scenario_music`, `main_sound_get_largest_free_block`, `main_sound_get_music_handle`, `main_sound_get_sfx_channels`, `main_sound_open_music_into_slot`, `main_sound_play_sfx_in_channel`, `main_sound_play_sfx_with_settings_in_channel`, `main_sound_set_current_music_balance_shift_timed`, `main_sound_set_current_music_pitch_shift_timed`, `main_sound_set_current_music_tempo_scale_timed`, `main_sound_set_sfx_balance`, `main_sound_stop_all`, `main_sound_stop_sfx_channel_pair`, `main_sound_sum_resource_words`, `main_sound_wait_for_scenario_smd_load`, `main_system_disable_root_counter_2_evcb`, `main_system_enable_root_counter_2_evcb`, `main_unit_init_stats_and_statuses`, `main_noop_800121cc` (and the same at 800121d4, 800124ac, 800124b4, 800124bc, 800124c4, 80013634, 80015864, 80018654, 800186bc, 80059ac0), `main_return_zero_80014204`. `main_set_display_draw_new_game` is called only by `main_gfx_fade_in_orbonne_monastery`. `main_sound_set_unread_value_800184e0` stores a word that `SuzukiSPUInitialiser` and `main_sound_quit` clear and nothing reads.
  - ATTACK (17): `attack_deploy_get_valid_tile`, `attack_deploy_init_valid_tiles`, `attack_deploy_set_arrow_position_mode`, `attack_deploy_set_cursor_tile`, `attack_deploy_set_valid_tile`, `attack_gfx_build_gradient_grid_primitives`, `attack_gfx_build_item_graphic_descriptor`, `attack_gfx_build_zodiac_primitives`, `attack_input_update_event_state`, `attack_menu_draw_thread_status_indicators`, `attack_panel_run_ability_list_thread`, `attack_panel_run_equipment_list_thread`, `attack_return_zero`, `attack_text_render_decimal_entry_list`, `attack_noop_801c087c` (and the same at 801c3070, 801c61dc).
  - BUNIT (13): `bunit_gfx_build_item_graphic_descriptor`, `bunit_gfx_get_transition_frame`, `bunit_gfx_move_image_and_wait`, `bunit_job_calculate_and_store_unlocks`, `bunit_menu_clear_selection_values`, `bunit_menu_icon_strip_thread`, `bunit_menu_init_and_draw_scrollable_list`, `bunit_menu_init_and_draw_scrollable_list_simple`, `bunit_menu_text_entry_wait_thread`, `bunit_panel_run_ability_list_thread`, `bunit_panel_run_equipment_list_thread`, `bunit_text_render_decimal_entry_list`, `bunit_noop_801c0adc`.
  - CARD (7): `card_gfx_enqueue_textured_quad_list`, `card_gfx_set_draw_color`, `card_menu_set_vsync_mode_and_event_speed`, `card_save_clear_selection_indices`, `card_text_count_encoded_glyphs`, `card_text_start_character_thread_if_idle`, `card_text_start_thread_if_idle`.
  - DEBUGCHR (12): `debugchr_gfx_build_gradient_grid_primitives`, `debugchr_gfx_build_item_graphic_descriptor`, `debugchr_gfx_build_zodiac_primitives`, `debugchr_input_update_event_state`, `debugchr_menu_draw_thread_status_indicators`, `debugchr_menu_icon_list_thread`, `debugchr_menu_icon_strip_thread`, `debugchr_menu_text_entry_wait_thread`, `debugchr_panel_run_ability_list_thread`, `debugchr_panel_run_equipment_list_thread`, `debugchr_text_render_decimal_entry_list`, `debugchr_noop_801c090c`.
  - EQUIP (17): `equip_gfx_enqueue_textured_quad_corners`, `equip_gfx_enqueue_textured_quad_list`, `equip_gfx_move_image_and_wait`, `equip_gfx_set_draw_color`, `equip_input_update_event_state`, `equip_menu_clear_selection_record`, `equip_menu_draw_thread_status_indicators`, `equip_menu_icon_strip_thread`, `equip_menu_init_and_draw_scrollable_list`, `equip_menu_init_and_draw_scrollable_list_simple`, `equip_menu_stop_threads`, `equip_menu_text_entry_wait_thread`, `equip_panel_run_ability_list_thread`, `equip_panel_run_character_status_thread`, `equip_set_s16_801d86ac`, `equip_unit_has_two_hands`, `equip_noop_801c151c`.
  - ETC (1): `etc_noop_801bf3d8`.
  - JOBSTTS (8): `jobstts_bits_init_secondary_reader`, `jobstts_bits_read_secondary`, `jobstts_gfx_enqueue_oriented_textured_quad`, `jobstts_gfx_enqueue_textured_quad_corners`, `jobstts_gfx_enqueue_textured_quad_list`, `jobstts_gfx_enqueue_textured_quad_rect`, `jobstts_menu_init_and_draw_scrollable_list`, `jobstts_menu_init_and_draw_scrollable_list_simple`.
  - OPTION (4): `option_gfx_build_scaled_draw_area_packets`, `option_gfx_init_scaled_draw_area_packets`, `option_menu_thin_window_frame_thread`, `option_thread_stop_after_yield`.
  - REQUIRE (15): `require_file_load_sync`, `require_gfx_build_item_graphic_descriptor`, `require_gfx_build_zodiac_primitives`, `require_input_update_event_state`, `require_input_wait_skip`, `require_menu_draw_thread_status_indicators`, `require_menu_icon_list_thread`, `require_menu_text_entry_wait_thread`, `require_panel_run_ability_list_thread`, `require_panel_run_equipment_list_thread`, `require_sound_play_music_tracks`, `require_text_render_decimal_entry_list`, `require_thread_stop_after_16_frames`, `require_noop_801c07ac` (and the same at 801c9fa4).
  - SMALL (1): `small_return_zero`.

## Pointers held in 32-bit integers

The PS1's pointers and `s32` are both 32 bits wide, so some code passes or
computes addresses as integers. A 64-bit build must give these pointer types.

- The thread parameters (`native_thread_t.function_parameter_1`..`_4`, and the
  `*_thread_get_current_parameter_*` getters) are `s32` but often carry record
  pointers, e.g. into `battle_script_run_sprite_move` and
  `world_menu_confirm_action_silently`.
- Matching spellings compute addresses through `(u32)` casts, e.g.
  `battle_script_get_variable_word_pointer_from_id` (offsets from
  `g_battle_script_variables`) and `battle_ai_load_known_ability_flag` (the
  learned-ability row).

