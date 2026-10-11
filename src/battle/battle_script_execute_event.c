#include "fft/battle.h"
#include "fft/event_attack.h"
#include "psx/pad.h"
#include "psx/types.h"

/* battle.h has no prototype for the Jump handler; this file and
 * battle_script_block_start_thread declare it. */
void battle_script_jump_event_instruction(battle_script_jump_instruction_t* instr);

/* BATTLE event-script interpreter: the body of the event thread
 * (NATIVE_THREAD_SLOT_EVENT), entered from battle_script_run_next_event once
 * battle_script_load_next_event has read the event block.
 *
 * Setup: with player input it waits for thread slots 2..13 and the camera to
 * go idle (in the other input modes it moves the four unit-stats displays to
 * y 0xAA). It then clears the status staging data, snapshots every unit's
 * statuses while the battle has not started (otherwise it runs the staged
 * inflict-status commands), and clears the disabled-menu, disabled-input and
 * forced-input variables.
 *
 * Each pass fetches the opcode at instruction_offset in g_battle_event_block
 * with its first four operand bytes and its first operand halfword,
 * dispatches through the else-if chain, then advances by
 * 1 + g_battle_script_event_instruction_sizes[opcode]. The jumps `continue`
 * without the length step; the Start instructions (camera fusion, block, add
 * unit) hand their body to a new thread and seek to the matching End. Most
 * long-running instructions start a helper thread on the first idle slot with
 * a pointer to their operands; work that belongs to the main loop is posted
 * in request globals that battle_script_process_pending_requests serves.
 *
 * EventEnd either chains into the event the scenario conditions select
 * (`goto restart_script`) or replays the staged statuses, settles the units'
 * facings and exits the thread. An opcode outside the chain, or a
 * ForwardTargetExit reached in sequence, also ends the thread.
 *
 * The chain order is retail's compare order. world_script_execute_event is
 * the WORLD copy of this function. BATTLE differs in five places: WalkTo
 * starts the unit's movement and Arrow sets an arrow trajectory (WORLD
 * recognises both and does nothing), DarkScreen's thread opens REQUIRE and
 * runs its image burst (WORLD's only takes the task id), the two ChangeMap
 * instructions reset the script camera transform, and the thread scans of
 * ResumeWaitingDialog and WaitForInstruction cover slots 1..15 where WORLD's
 * cover 1..16.
 *
 * battle_thread_call_on_main_stack is declared without a parameter list: it
 * forwards whatever arguments its current call target takes. */
void battle_script_execute_event(void) {
    s32 added_unit_state;
    u8* message_cursor;
    u8* effect_cursor;
    s32 finish_operation;
    s16 first_halfword;
    s16 remove_unit_misc_id;
    s16 reset_graphic_misc_id;
    s16 shadow_misc_id;
    s16 evtchr_palette_misc_id;
    s16 thrown_item_misc_id;
    s16 arrow_target_misc_id;
    s16 arrow_shooter_misc_id;
    s32 ghost_misc_id;
    s32 region_misc_id;
    s16 draw_misc_id;
    s32 date_month;
    battle_stats_t* added_unit;
    s32 arrow_shooter_battle_id;
    s32 confirm_buttons;
    s32 instruction_offset;
    s32 added_battle_id;
    s32 work_index;
    event_opcode_e opcode;
    /* Initially four eagerly fetched operand bytes, then per-opcode scratch.
     * They are not four uniformly encoded script arguments. */
    s32 operand_2;
    s32 operand_3;
    s32 operand_4;
    s32 operand_1;
    u8* position_instruction;
    u8* message_instruction;
    s32* script_variables;
    u8* parameters;
    /* One unit pointer, one thread pointer and one index serve every handler;
     * separate ones per handler change the saved-register allocation. */
    battle_stats_t* unit;
    u8* ghost_parameters;
    u8* instruction;
    map_tile_t* tile;
    battle_stats_t* copy_destination;
    u8* effect_instruction;
    native_thread_t* thread;
    native_thread_t* threads;

    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_EXECUTE_EVENT);
    if (battle_script_is_tutorial_event_slot() != 0) {
        battle_gfx_ramp_screen_overlay_intensity_to_zero();
    }
    if (g_battle_menu_input_mode == MENU_INPUT_PLAYER) {
        for (work_index = 2; work_index < 0xE; work_index++) {
            battle_thread_wait_until_inactive(work_index);
        }
        battle_camera_wait_until_idle();
    } else {
        g_battle_menu_hovered_unit_stats_display.y = 0xAA;
        g_battle_menu_attack_caster_stats_display.y = 0xAA;
        g_battle_menu_attack_target_stats_display.y = 0xAA;
        g_battle_menu_right_unit_stats_display.y = 0xAA;
    }
    battle_unit_clear_status_staging_data();
    if (battle_script_get_variable(EVENT_SCRIPT_VAR_BATTLE_NOT_STARTED) != 0) {
        battle_unit_update_staged_status_data(0, 0);
    } else {
        battle_script_process_inflict_status_commands();
    }
    battle_script_set_variable(EVENT_SCRIPT_VAR_DISABLED_MENU_ACTIONS, 0);
    battle_script_set_variable(EVENT_SCRIPT_VAR_DISABLED_CONTROLLER_INPUTS, 0);
    battle_script_set_variable(EVENT_SCRIPT_VAR_FORCED_CONTROLLER_INPUTS, 0);
restart_script:
    battle_script_set_event_speed(1U);
    instruction_offset = 0;
    battle_camera_normalize_yaw_angle();
    {
        s32 empty_unit_id = EVENT_UNIT_SLOT_EMPTY;
        /* Matching: load the fill value before the descending loop index. */
        for (work_index = 3; work_index >= 0; work_index--) {
            g_battle_event_unit_slots[work_index] = empty_unit_id;
        }
    }
    for (;;) {
        instruction = (u8*)g_battle_event_block;
        instruction += instruction_offset;
        parameters = instruction + 1;
        operand_1 = instruction[1];
        operand_2 = instruction[2];
        operand_3 = instruction[3];
        operand_4 = instruction[4];
        opcode = instruction[0];
        first_halfword = battle_script_load_halfword(parameters);
        /* ---- Control: NOPs, conditions, and script variable arithmetic ---- */
        if (opcode == EVENT_OPCODE_CAMERA_DISABLED) {
        } else if (opcode == EVENT_OPCODE_NOP) {
        } else if ((opcode - EVENT_CONDITION_SECOND_GTE_FIRST) < 6U) {
            battle_script_run_condition(opcode);
        } else if ((opcode - EVENT_OPCODE_ADD_IMMEDIATE) < 0xFU) {
            operand_1 = first_halfword & 0xFFFF;
            battle_script_run_variable_command(opcode, operand_1,
                battle_script_load_halfword(instruction_offset + ((u8*)g_battle_event_block) + 3) & 0xFFFF, 0);
        } else if (opcode == EVENT_OPCODE_REQUEST_MAP_JUMP_IN) {
            g_battle_event_map_jump_in_request = operand_1;
            battle_thread_wait_frames(2);
        }
        /* ---- Scenario progression: DismissUnit, CallFunction, BlueRemoveUnit, EventEnd ---- */
        else if (opcode == EVENT_OPCODE_DISMISS_UNIT) {
            if (battle_script_get_variable(EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS) == 0) {
                battle_script_dismiss_unit_event_instruction(first_halfword);
            }
        } else if (opcode == EVENT_OPCODE_CALL_FUNCTION) {
            /* These are sequential tests, not switch alternatives: selectors
             * 6 and 14 reuse operand_1 as a loop index, and later tests see
             * its changed value. Preserve that target-visible fallthrough. */
            battle_thread_wait_frames(4);
            if (battle_script_get_variable(EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS) == 0) {
                if (operand_1 == EVENT_CALL_FUNCTION_JOIN_UNITS_SILENTLY) {
                    if (battle_script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT) < EVENT_ID_NEW_GAME_INVENTORY_END) {
                        main_item_init_new_game_inventory();
                    }
                    battle_script_join_units_silently();
                }
                if (operand_1 == EVENT_CALL_FUNCTION_OPEN_SAVE_MENU) {
                    operand_1 = 2;
                    do {
                        battle_thread_set_parameters(operand_1, 0U, 0, 1);
                        g_battle_thread_contexts[operand_1].task_id = NATIVE_THREAD_TASK_STOP_REQUEST;
                        battle_thread_wait_until_inactive(operand_1);
                        operand_1 += 1;
                    } while (operand_1 < 0xF);
                    if (battle_script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT) != EVENT_ID_EPILOGUE) {
                        battle_script_set_variable(EVENT_SCRIPT_VAR_NEXT_SCENARIO, 1);
                    }
                    battle_script_set_variable(EVENT_SCRIPT_VAR_SAVE_IN_PROGRESS, 1);
                    battle_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_CARD);
                    g_battle_companion_overlay_state = BATTLE_COMPANION_OVERLAY_STATE_CARD_SAVE;
                    do {
                        battle_thread_yield();
                    } while (g_battle_companion_overlay_state != BATTLE_COMPANION_OVERLAY_STATE_IDLE);
                    battle_script_set_variable(EVENT_SCRIPT_VAR_NEXT_SCENARIO, 0);
                    operand_1 = 2;
                    do {
                        battle_thread_set_parameters(operand_1, 0U, 0, 1);
                        g_battle_thread_contexts[operand_1].task_id = NATIVE_THREAD_TASK_STOP_REQUEST;
                        battle_thread_wait_until_inactive(operand_1);
                        operand_1 += 1;
                    } while (operand_1 < 0xF);
                }
            }
            if ((operand_1 == EVENT_CALL_FUNCTION_ADVANCE_YEAR)
                || (operand_1 == EVENT_CALL_FUNCTION_ADVANCE_YEAR_ALTERNATE)) {
                battle_script_advance_date_by_one_year();
            }
            if (operand_1 == EVENT_CALL_FUNCTION_INCREASE_BLUE_TEAM_BRAVE) {
                battle_unit_increase_blue_team_brave_by_10();
            }
            if (operand_1 == EVENT_CALL_FUNCTION_REMOVE_ALL_ENEMY_UNITS) {
                battle_script_blue_remove_all_enemy_units();
                battle_thread_wait_frames(0x78);
            }
            if (operand_1 == EVENT_CALL_FUNCTION_REQUEST_CAMERA_YAW_WRAP) {
                g_battle_camera_wrap_yaw_pending = 1;
            }
            if (operand_1 == EVENT_CALL_FUNCTION_DISABLE_DEBUG_DISPLAY) {
                g_main_debug_display_enabled = 0;
                D_800459D8 = 0;
            }
            if (operand_1 == EVENT_CALL_FUNCTION_MUTE_TEXT_AUDIO_CUE) {
                battle_script_set_variable(EVENT_SCRIPT_VAR_MUTE_TEXT_AUDIO_CUE, 1);
            }
            if (operand_1 == EVENT_CALL_FUNCTION_UNMUTE_TEXT_AUDIO_CUE) {
                battle_script_set_variable(EVENT_SCRIPT_VAR_MUTE_TEXT_AUDIO_CUE, 0);
            }
            if (operand_1 == EVENT_CALL_FUNCTION_ADVANCE_MAP_DESTRUCTION_AND_WAIT) {
                battle_script_set_specialized_map_destroyed();
                if (g_battle_map_destruction_wait_state != 0) {
                    do {
                        battle_thread_yield();
                    } while (g_battle_map_destruction_wait_state != 0);
                }
            }
            if (operand_1 == EVENT_CALL_FUNCTION_PLAY_ALTIMA_MUSIC) {
                g_battle_current_music_track_index = 0;
                g_battle_music_track_1_id = MAIN_MUSIC_ALTIMA;
                g_battle_music_track_2_id = 0;
                battle_sound_play_music_tracks(MAIN_MUSIC_ALTIMA, MAIN_MUSIC_NONE);
            }
            if (operand_1 == EVENT_CALL_FUNCTION_COPY_UNIT_8_POSITION_TO_UNIT_9_AND_RELOAD_GRAPHICS) {
                unit = battle_unit_get_stats_from_battle_id(7);
                copy_destination = battle_unit_get_stats_from_battle_id(8);
                copy_destination->x = unit->x;
                copy_destination->position.bits.y = unit->position.bits.y;
                copy_destination->position.bits.higher_elevation = unit->position.bits.higher_elevation;
                battle_gfx_load_unit_graphics_by_battle_id(8, 1U);
                g_battle_unit_graphics_load_pending = 1;
                do {
                    battle_thread_yield();
                } while (g_battle_unit_graphics_load_pending != 0);
                battle_thread_yield();
                battle_thread_yield();
            }
            if (operand_1 == EVENT_CALL_FUNCTION_RESET_BETHLA_REGION_UNIT_GRAPHICS) {
                region_misc_id = 0;
                do {
                    if (battle_unit_has_misc_id(region_misc_id) != 0) {
                        operand_1 = battle_unit_get_battle_index_by_misc_id(region_misc_id);
                        if (operand_1 != -1U) {
                            unit = battle_unit_get_stats_from_battle_id(operand_1);
                            if ((unit->x < 4U) && (unit->position.bits.y < 0xBU)) {
                                battle_gfx_reset_unit_graphic_trigger(region_misc_id);
                            }
                            if (((unit->x - 4) < 3U) && ((unit->position.bits.y - 2) < 6U)) {
                                battle_gfx_reset_unit_graphic_trigger(region_misc_id);
                            }
                        }
                    }
                    region_misc_id += 1;
                } while (region_misc_id < EVENT_UNIT_SLOT_COUNT);
            }
            if (operand_1 == EVENT_CALL_FUNCTION_COPY_UNIT_2_POSITION_AND_FACING_TO_UNIT_8_AND_RELOAD_GRAPHICS) {
                unit = battle_unit_get_stats_from_battle_id(1);
                copy_destination = battle_unit_get_stats_from_battle_id(7);
                copy_destination->x = unit->x;
                copy_destination->position.bits.y = unit->position.bits.y;
                copy_destination->position.bits.higher_elevation = unit->position.bits.higher_elevation;
                copy_destination->position.bits.facing = unit->position.bits.facing;
                battle_gfx_load_unit_graphics_by_battle_id(7, 1U);
                g_battle_unit_graphics_load_pending = 1U;
                do {
                    battle_thread_yield();
                } while (g_battle_unit_graphics_load_pending != 0);
                battle_thread_yield();
                battle_thread_yield();
            }
            if ((operand_1 - EVENT_CALL_FUNCTION_WARP_UNIT_4_DISPLAY_TO_UNIT_2_POSITION_AND_FACING) < 2U) {
                battle_script_warp_unit_display_to_paired_unit(operand_1);
            }
            if (operand_1 == EVENT_CALL_FUNCTION_CLEAR_ZALERA_ACTIVE_STATUS_GRAPHICS) {
                draw_misc_id = battle_script_get_misc_id(CHARACTER_IDENTITY_ZALERA);
                if (draw_misc_id != EVENT_MISC_ID_NONE) {
                    battle_status_queue_current_status_graphics(
                        battle_unit_get_battle_index_by_misc_id(draw_misc_id), 0);
                    g_battle_thread_call_target = (void (*)(void))battle_unit_update_graphics_by_misc_id_wrapper;
                    battle_thread_call_on_main_stack(draw_misc_id);
                }
            }
        } else if (opcode == EVENT_OPCODE_BLUE_REMOVE_UNIT) {
            battle_script_blue_remove_unit(first_halfword);
        } else if (opcode == EVENT_OPCODE_END || opcode == EVENT_OPCODE_END_2) {
            battle_map_wait_for_refresh();
            do {
                battle_thread_yield();
            } while (battle_thread_find_running_by_task(NATIVE_THREAD_TASK_EVENT_BLOCK) != 0);
            do {
                battle_thread_yield();
            } while (main_file_is_still_loading() != 0);
            battle_thread_yield();
            if (battle_script_is_tutorial_event_slot() != 0) {
                battle_gfx_ramp_screen_overlay_intensity_to_max();
                g_main_game_options = g_battle_saved_game_options;
                battle_script_set_variable(EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS, 0);
                battle_state_start_close_battle(0x20);
            }
            if (g_battle_menu_input_mode == MENU_INPUT_PLAYER) {
                for (work_index = 2; work_index < 0xE; work_index++) {
                    battle_thread_wait_until_inactive(work_index);
                }
            }
            operand_1 = battle_script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT);
            if (operand_1 != EVENT_ID_OLAN_WOUNDED && operand_1 != EVENT_ID_BEOWULF_INVITATION) {
                battle_gfx_init_evtchr_vram_slots();
            }
            battle_script_set_variable(EVENT_SCRIPT_VAR_BATTLE_NOT_STARTED, 0);
            /* An event with no after-event code, outside events
             * 0x190..0x1a9, checks the scenario conditions and chains into
             * the event they select. */
            finish_operation = (g_main_scenario_event_finish_operations[operand_1] & ~0x0CFF) >> 8;
            if (g_battle_menu_input_mode == MENU_INPUT_PLAYER && finish_operation == 0
                && (operand_1 - EVENT_ID_COMMON_FIRST) >= 0x1AU && operand_1 != 0) {
                g_battle_script_condition_unit_battle_id = 0xFF;
                battle_script_run_scenario_conditions();
                if (battle_script_load_next_event() != 0) {
                    goto restart_script;
                }
            }
            /* Otherwise the event ends: replay the staged statuses and copy
             * each displayed unit's facing quadrant to its battle record. */
            work_index = 0;
            battle_unit_snap_all_facings_to_quadrants();
            battle_unit_update_staged_status_data(0, 1);
            do {
                if (battle_unit_has_misc_id(work_index) != 0) {
                    operand_1 = battle_unit_get_battle_index_by_misc_id(work_index);
                    if (operand_1 >= 0) {
                        unit = battle_unit_get_stats_from_battle_id(operand_1);
                        unit->position.bits.facing = battle_unit_get_facing_quadrant_by_misc_id(work_index);
                    }
                }
                work_index += 1;
            } while (work_index < EVENT_UNIT_SLOT_COUNT);
            battle_camera_normalize_yaw_angle();
            battle_camera_init_tilt_and_zoom();
            battle_thread_exit_current();
        }
        /* ---- Flow control: forward and backward jumps ---- */
        else if (opcode == EVENT_OPCODE_UNKNOWN_DA) {
        } else if (opcode == EVENT_OPCODE_JUMP_FORWARD_IF_ZERO) {
            if (g_battle_script_variables[EVENT_SCRIPT_VAR_COMPARISON_RESULT] == 0) {
                instruction_offset = battle_script_find_jump_target(
                    instruction_offset + 2, EVENT_OPCODE_FORWARD_TARGET, EVENT_OPCODE_FORWARD_TARGET_EXIT, operand_1);
                continue;
            }
        } else if (opcode == EVENT_OPCODE_JUMP_FORWARD) {
            instruction_offset
                = battle_script_find_jump_target(instruction_offset + 2, EVENT_OPCODE_FORWARD_TARGET, -1, operand_1);
            continue;
        } else if (opcode == EVENT_OPCODE_FORWARD_TARGET) {
        } else if (opcode == EVENT_OPCODE_FORWARD_TARGET_EXIT) {
            break;
        } else if (opcode == EVENT_OPCODE_JUMP_BACK) {
            instruction_offset
                = battle_script_find_jump_target(instruction_offset, EVENT_OPCODE_BACK_TARGET, -1, operand_1);
            continue;
        } else if (opcode == EVENT_OPCODE_BACK_TARGET) {
        } else if (opcode == EVENT_OPCODE_UNKNOWN_D8) {
        } else if (opcode == EVENT_OPCODE_BACK_TARGET_BARRIER) {
        } else if (opcode == EVENT_OPCODE_UNKNOWN_DC) {
            g_battle_script_opcode_dc_value = 0xD;
        }
        /* ---- Dialogue and portraits ---- */
        else if (opcode == EVENT_OPCODE_DISPLAY_MESSAGE) {
            operand_1 = battle_thread_resolve_id(operand_1);
            battle_thread_start(operand_1, battle_text_character_handling_thread);
            battle_text_build_display_message(
                instruction_offset, operand_2, &g_battle_thread_contexts[operand_1].task_words[6]);
            g_battle_thread_contexts[operand_1].function_parameter_1 = operand_2;
            g_battle_thread_contexts[operand_1].function_parameter_2 = ((operand_4 << 8) + operand_3) - 1;
            message_instruction = (u8*)g_battle_event_block;
            message_instruction += instruction_offset;
            operand_4 = message_instruction[6];
            operand_3 = message_instruction[5];
            g_battle_thread_contexts[operand_1].function_parameter_3 = (operand_4 << 8) + operand_3;
            g_battle_thread_contexts[operand_1].function_parameter_4 = message_instruction[7];
            g_battle_thread_contexts[operand_1].task_words[2] = battle_script_load_halfword(message_instruction + 8);
            g_battle_thread_contexts[operand_1].task_words[3]
                = battle_script_load_halfword(instruction_offset + ((u8*)g_battle_event_block) + 0xA);
            g_battle_thread_contexts[operand_1].task_words[4]
                = battle_script_load_halfword(instruction_offset + ((u8*)g_battle_event_block) + 0xC);
            message_cursor = (u8*)g_battle_event_block;
            message_cursor += instruction_offset;
            g_battle_thread_contexts[operand_1].task_words[5] = message_cursor[14];
            battle_thread_yield();
        } else if (opcode == EVENT_OPCODE_CHANGE_DIALOG) {
            for (work_index = 0; work_index < 6; work_index += 2) {
                operand_2 = g_battle_menu_window_buffers[work_index].thread_id;
                if ((g_battle_menu_window_buffers[work_index].dialogue_selector == operand_1)
                    && (battle_thread_is_running_asm(operand_2) == 1)
                    && (g_battle_thread_contexts[operand_2].task_id == NATIVE_THREAD_TASK_DIALOG_AWAIT_TEXT)) {
                    operand_1 = battle_script_load_halfword(instruction_offset + ((u8*)g_battle_event_block) + 2);
                    if ((operand_1 & 0xFFFF) != 0xFFFF) {
                        operand_1 -= 1;
                    }
                    g_battle_thread_contexts[operand_2].function_parameter_2 = operand_1;
                    g_battle_thread_contexts[operand_2].function_parameter_4
                        = battle_script_load_halfword(instruction_offset + ((u8*)g_battle_event_block) + 4);
                    g_battle_thread_contexts[operand_2].task_id = NATIVE_THREAD_TASK_RESUME;
                    break;
                }
            }
        } else if (opcode == EVENT_OPCODE_PORTRAIT_COL) {
            battle_script_load_portrait_colors_event_instruction(operand_1);
        }
        /* ---- Map objects ---- */
        else if (opcode == EVENT_OPCODE_USE_3D_OBJECT) {
            g_battle_3d_object_use_request = 1;
            g_battle_event_3d_object_id = operand_1;
            g_battle_event_3d_object_state = operand_2;
            battle_thread_yield();
        } else if (opcode == EVENT_OPCODE_WAIT_3D_OBJECT) {
            g_battle_3d_object_wait_status = 1;
            do {
                battle_thread_yield();
            } while (g_battle_3d_object_wait_status != 0);
        } else if (opcode == EVENT_OPCODE_USE_FIELD_OBJECT) {
            g_battle_field_object_use_request = 1;
            g_battle_event_map_command_83_arg1 = operand_1;
            g_battle_event_map_command_83_arg2 = operand_2;
            battle_thread_yield();
        } else if (opcode == EVENT_OPCODE_WAIT_FIELD_OBJECT) {
            g_battle_field_object_wait_status = 1;
            do {
                battle_thread_yield();
            } while (g_battle_field_object_wait_status != 0);
        }
        /* ---- Unit sprites and animation ---- */
        else if (opcode == EVENT_OPCODE_MIRROR_SPRITE) {
            battle_script_mirrorsprite_event_instruction((const event_mirror_sprite_parameters_t*)parameters);
        } else if (opcode == EVENT_OPCODE_RESET_PALETTE) {
            battle_script_resetpalette_event_instruction((const event_unit_parameters_t*)parameters);
        } else if (opcode == EVENT_OPCODE_SET_FIRST_MISC_UNIT) {
            operand_1 = battle_script_get_misc_id(first_halfword);
            if (operand_1 != EVENT_MISC_ID_NONE) {
                battle_unit_move_misc_unit_to_head(operand_1);
            }
        } else if (opcode == EVENT_OPCODE_UNIT_ANIM) {
            battle_script_unit_anim((const event_unit_anim_parameters_t*)parameters);
        } else if (opcode == EVENT_OPCODE_MARCH) {
            battle_script_march_units((const event_march_parameters_t*)parameters);
        } else if (opcode == EVENT_OPCODE_SET_UNIT_MOVEMENT_EFFECT_SUPPRESSION) {
            battle_script_set_units_movement_effect_suppression((const event_movement_effect_parameters_t*)parameters);
        } else if (opcode == EVENT_OPCODE_WAIT_UNIT_READY) {
            operand_1 = battle_script_get_misc_id(first_halfword);
            if (operand_1 != EVENT_MISC_ID_NONE) {
                battle_script_wait_for_unit_ready(operand_1);
            }
        }
        /* ---- Map state, weather, and time of day ---- */
        else if (opcode == EVENT_OPCODE_CHANGE_MAP_BETA) {
            g_battle_event_pending_map_jump_out_id = first_halfword;
            battle_thread_wait_frames(2);
            battle_map_reset_script_light_transform();
        } else if (opcode == EVENT_OPCODE_CHANGE_MAP) {
            g_battle_event_pending_map_jump_out_2_id = first_halfword;
            battle_thread_wait_frames(2);
            battle_map_reset_script_light_transform();
        } else if (opcode == EVENT_OPCODE_SET_DAYTIME) {
            g_battle_event_pending_map_state = operand_1;
            battle_thread_wait_frames(2);
            battle_map_wait_for_refresh();
        } else if (opcode == EVENT_OPCODE_WEATHER) {
            g_battle_event_weather_request = first_halfword;
            battle_thread_wait_frames(2);
            battle_map_wait_for_refresh();
        } else if (opcode == EVENT_OPCODE_RELOAD_MAP_STATE) {
            g_battle_map_reload_state = 1;
            do {
                battle_thread_yield();
            } while (g_battle_map_reload_state != 0);
        } else if (opcode == EVENT_OPCODE_APPEND_MAP_STATE) {
            g_battle_map_append_state = 1;
            do {
                battle_thread_yield();
            } while (g_battle_map_append_state != 0);
        } else if (opcode == EVENT_OPCODE_PAUSE) {
            battle_script_pause_event_instruction();
        } else if (opcode == EVENT_OPCODE_RESUME_WAITING_DIALOG) {
            for (work_index = 1; work_index < 16; work_index++) {
                thread = &g_battle_thread_contexts[work_index];
                if (thread->is_running != 0 && thread->task_id == NATIVE_THREAD_TASK_WAIT_FOR_RESUME
                    && thread->function_parameter_2 == first_halfword) {
                    thread->task_id = NATIVE_THREAD_TASK_RESUME;
                }
            }
        } else if (opcode == EVENT_OPCODE_FLIP_MESSAGE_PORTRAIT) {
            battle_script_toggle_message_portrait_flip(parameters);
        } else if (opcode == EVENT_OPCODE_UNKNOWN_17) {
        }
        /* ---- Effects ---- */
        else if (opcode == EVENT_OPCODE_EFFECT) {
            g_battle_event_pending_effect_id = first_halfword;
            effect_cursor = (u8*)g_battle_event_block;
            effect_cursor += instruction_offset;
            g_battle_event_effect_target_misc_id = battle_script_get_misc_id(effect_cursor[3]);
            effect_instruction = (u8*)g_battle_event_block;
            effect_instruction += instruction_offset;
            g_battle_event_effect_target_x = effect_instruction[4];
            g_battle_event_effect_target_y = effect_instruction[5];
            g_battle_event_effect_target_mode = effect_instruction[6];
            battle_script_set_variable(EVENT_SCRIPT_VAR_CAMERA_ROLL, 0);
            battle_thread_wait_frames(2);
        } else if (opcode == EVENT_OPCODE_EFFECT_START) {
            do {
                battle_thread_yield();
                g_battle_thread_call_target = (void (*)(void))battle_effect_try_init_data;
            } while (battle_thread_call_on_main_stack() != 0);
        } else if (opcode == EVENT_OPCODE_EFFECT_END) {
            battle_thread_start(
                battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE), &battle_script_play_effect_thread);
        }
        /* ---- Camera ---- */
        else if (opcode == EVENT_OPCODE_FOCUS) {
            battle_camera_store_yaw_band(g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW]);
            battle_script_focus((event_focus_parameters_t*)parameters);
        } else if (opcode == EVENT_OPCODE_FOCUS_SPEED) {
            script_variables = g_battle_script_variables;
            battle_script_focus_speed(parameters, &script_variables[EVENT_SCRIPT_VAR_CAMERA_X],
                &script_variables[EVENT_SCRIPT_VAR_CAMERA_TILT]);
        } else if (opcode == EVENT_OPCODE_RELATIVE_CAMERA) {
            battle_script_apply_relative_camera(
                (event_camera_values_t*)parameters, &g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_X]);
        } else if (opcode == EVENT_OPCODE_CAMERA) {
            operand_1 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_1, &battle_camera_thread);
            g_battle_thread_contexts[operand_1].function_parameter_1 = (s32)parameters;
        } else if (opcode == EVENT_OPCODE_CAMERA_FUSION_START) {
            operand_1 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_1, battle_camera_fusion_thread);
            g_battle_thread_contexts[operand_1].function_parameter_1 = (s32)parameters;
            instruction_offset
                = battle_script_find_instruction_byte_offset(instruction_offset, EVENT_OPCODE_CAMERA_FUSION_END);
        } else if (opcode == EVENT_OPCODE_CAMERA_FUSION_END) {
        } else if (opcode == EVENT_OPCODE_CAMERA_SPEED_CURVE) {
            g_battle_camera_speed_curve = operand_1;
        } else if (opcode == EVENT_OPCODE_UNKNOWN_74) {
            battle_script_opcode_74_event_instruction(parameters);
        }
        /* ---- Unit movement and facing ---- */
        else if (opcode == EVENT_OPCODE_WALK_TO) {
            g_battle_thread_call_target = (void (*)(void))battle_script_walkto_event_instruction;
            battle_thread_call_on_main_stack(parameters);
        } else if (opcode == EVENT_OPCODE_JUMP) {
            battle_script_jump_event_instruction((battle_script_jump_instruction_t*)parameters);
        } else if (opcode == EVENT_OPCODE_WAIT_WALK) {
            battle_script_waitwalk_event_instruction(first_halfword);
        } else if (opcode == EVENT_OPCODE_WALK_TO_ANIM) {
            operand_1 = battle_script_get_misc_id(first_halfword);
            if (operand_1 != EVENT_MISC_ID_NONE) {
                g_battle_unit_pending_animation_ids[operand_1]
                    = battle_script_load_halfword(instruction_offset + ((u8*)g_battle_event_block) + 3);
                g_battle_unit_was_moving_latch[operand_1] = 0;
            }
        } else if (opcode == EVENT_OPCODE_WAIT_ALL_UNIT_MOVEMENT) {
            do {
                battle_thread_yield();
            } while (battle_unit_is_moving(-1) != 0);
        } else if (opcode == EVENT_OPCODE_WAIT_FILE_LOAD) {
            do {
                battle_thread_yield();
            } while (main_file_is_still_loading() != 0);
        } else if (opcode == EVENT_OPCODE_ROTATE_UNIT) {
            battle_script_rotate_unit((const event_rotate_unit_parameters_t*)parameters);
        } else if (opcode == EVENT_OPCODE_FACE_EACH_OTHER) {
            battle_script_face_unit((const event_face_unit_parameters_t*)parameters, 0);
        } else if (opcode == EVENT_OPCODE_FACE_UNIT) {
            battle_script_face_unit((const event_face_unit_parameters_t*)parameters, 1);
        } else if (opcode == EVENT_OPCODE_FACE_TILE) {
            battle_script_face_tile((const event_face_tile_parameters_t*)parameters);
        } else if (opcode == EVENT_OPCODE_WAIT_ROTATE_UNIT) {
            battle_script_waitrotateunit_and_waitrotateall_event_instruction(first_halfword);
        } else if (opcode == EVENT_OPCODE_WAIT_ROTATE_ALL) {
            battle_script_waitrotateunit_and_waitrotateall_event_instruction(-1);
        }
        /* ---- Blocks, earthquakes, darkness, and screen color ---- */
        else if (opcode == EVENT_OPCODE_BLOCK_START) {
            operand_1 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_1, battle_script_block_start_thread);
            g_battle_thread_contexts[operand_1].function_parameter_1 = (s32)parameters;
            instruction_offset = battle_script_find_instruction_byte_offset(instruction_offset, EVENT_OPCODE_BLOCK_END);
        } else if (opcode == EVENT_OPCODE_BLOCK_END) {
        } else if (opcode == EVENT_OPCODE_UNKNOWN_23) {
        } else if (opcode == EVENT_OPCODE_UNKNOWN_24) {
        } else if (opcode == EVENT_OPCODE_EARTHQUAKE_END) {
            battle_thread_wait_frames(1);
            battle_thread_suspend(battle_thread_find_running_by_task(NATIVE_THREAD_TASK_EARTHQUAKE));
        } else if (opcode == EVENT_OPCODE_SET_SPEAKER_PAIR) {
            g_battle_text_speaker_swap_unit_a = first_halfword;
            g_battle_text_speaker_swap_unit_b
                = battle_script_load_halfword(instruction_offset + ((u8*)g_battle_event_block) + 3);
        } else if (opcode == EVENT_OPCODE_MAP_DARKNESS) {
            operand_1 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_1, &battle_script_map_darkness);
            g_battle_thread_contexts[operand_1].function_parameter_1 = (s32)parameters;
        } else if (opcode == EVENT_OPCODE_COLOR_SCREEN) {
            operand_1 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_1, &battle_script_color_screen_thread);
            g_battle_thread_contexts[operand_1].function_parameter_1 = (s32)parameters;
        } else if (opcode == EVENT_OPCODE_EARTHQUAKE_START) {
            operand_1 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_1, &battle_script_earthquake_start);
            g_battle_thread_contexts[operand_1].function_parameter_1 = (s32)parameters;
        } else if ((opcode == EVENT_OPCODE_SPRITE_MOVE) || (opcode == EVENT_OPCODE_SPRITE_MOVE_BETA)) {
            operand_1 = battle_thread_resolve_id_after_current(NATIVE_THREAD_SLOT_FIRST_IDLE);
            if (opcode == EVENT_OPCODE_SPRITE_MOVE) {
                battle_thread_start(operand_1, &battle_script_sprite_move);
            } else {
                battle_thread_start(operand_1, &battle_script_sprite_move_beta);
            }
            g_battle_thread_contexts[operand_1].function_parameter_1 = (s32)parameters;
            g_battle_thread_contexts[operand_1].task_id = NATIVE_THREAD_TASK_SPRITE_MOVE;
            g_battle_thread_contexts[operand_1].task_words[0] = battle_script_get_misc_id(first_halfword);
        } else if (opcode == EVENT_OPCODE_WAIT_SPRITE_MOVE) {
            battle_script_waitspritemove_event_instruction(first_halfword);
        } else if (opcode == EVENT_OPCODE_MAP_LIGHT) {
            operand_1 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_1, &battle_map_light_thread);
            g_battle_thread_contexts[operand_1].function_parameter_1 = (s32)parameters;
        }
        /* ---- Background, palettes, and terrain ---- */
        else if (opcode == EVENT_OPCODE_BACKGROUND) {
            battle_copy_bytes(&g_battle_event_background_colors.colors[MAP_BACKGROUND_GRADIENT_TOP], parameters, 3);
            battle_copy_bytes(
                &g_battle_event_background_colors.colors[MAP_BACKGROUND_GRADIENT_BOTTOM], parameters + 3, 3);
            if (parameters[7] == 0) {
                battle_map_transition_and_store_background_gradient(parameters[6], &g_battle_event_background_colors);
            } else {
                battle_map_set_background_gradient(parameters[6], &g_battle_event_background_colors);
            }
        } else if (opcode == EVENT_OPCODE_COLOR_BG_BETA) {
            battle_map_modify_background_gradient(
                parameters[0], parameters[4], ((s8)parameters[1]), ((s8)parameters[2]), ((s8)parameters[3]));
        } else if (opcode == EVENT_OPCODE_COLOR_UNIT) {
            battle_script_color_unit_event_instruction((const event_color_unit_parameters_t*)parameters);
        } else if (opcode == EVENT_OPCODE_RESUME_UNIT_PALETTE_UPDATES) {
            battle_script_set_units_palette_update_suppression(first_halfword & 0xFFFF, 0);
        } else if (opcode == EVENT_OPCODE_SUPPRESS_UNIT_PALETTE_UPDATES) {
            battle_script_set_units_palette_update_suppression(first_halfword & 0xFFFF, 1);
        } else if (opcode == EVENT_OPCODE_COLOR_FIELD) {
            battle_map_color_field(
                parameters[0], parameters[4], ((s8)parameters[1]), ((s8)parameters[2]), ((s8)parameters[3]));
        } else if (opcode == EVENT_OPCODE_STORE_MODIFIED_MAP_PALETTE) {
            battle_map_store_modified_palette_colors(0, 0, 1);
        } else if (opcode == EVENT_OPCODE_SET_TILE_CAMERA_BLOCK) {
            battle_map_set_tile_data_value(
                MAP_TILE_DATA_FLAT_CAMERA_BLOCK_MASK, parameters[0], parameters[1], parameters[2], parameters[3]);
        } else if (opcode == EVENT_OPCODE_SET_TILE_HEIGHT) {
            tile = battle_map_get_tile_data_pointer(parameters[0], parameters[1], parameters[2]);
            if ((tile->surface.bits.type - MAP_SURFACE_GRASSLAND) >= 2U) {
                tile->surface.value = (tile->surface.value & ~MAP_SURFACE_MASK) | MAP_SURFACE_STONE_FLOOR;
            }
            tile->height = parameters[3];
            tile->flags.bits.untargetable = parameters[4];
            tile->flags.bits.blocked = 0;
        }
        /* ---- Event speed, dark screen, conditions, and graphics ---- */
        else if (opcode == EVENT_OPCODE_EVENT_SPEED) {
            battle_script_set_event_speed(operand_1);
        } else if (opcode == EVENT_OPCODE_DARK_SCREEN) {
            g_battle_menu_hide_numeric_values = 0;
            operand_1 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_1, &battle_script_dark_screen_thread);
            battle_thread_set_parameters(operand_1, 0, (s32)parameters, 0);
        } else if (opcode == EVENT_OPCODE_REMOVE_DARK_SCREEN) {
            operand_1 = battle_thread_find_running_by_task(NATIVE_THREAD_TASK_DARK_SCREEN_HOLD);
            if (operand_1 != 0) {
                g_battle_thread_contexts[operand_1].task_id = NATIVE_THREAD_TASK_DARK_SCREEN;
            }
        } else if (opcode == EVENT_OPCODE_DISPLAY_CONDITIONS) {
            operand_3 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            if ((operand_3 == 4) && (operand_1 == EVENT_DISPLAY_CONDITION_APPLY_POST_BATTLE_UNIT_CHANGES)) {
                battle_thread_wait_until_inactive(2);
                operand_3 = 2;
            }
            battle_thread_start(operand_3, &battle_script_execute_display_conditions_instruction);
            battle_thread_set_parameters(operand_3, operand_1, operand_2, 0);
        } else if (opcode == EVENT_OPCODE_SHOW_GRAPHIC) {
            /* Use: the extra operand_2 reference keeps operand_2/operand_3 in $s5/$s6, not swapped. */
            __asm__("" : : "r"(operand_2));
            operand_3 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_3, &battle_script_show_graphic);
            battle_thread_set_parameters(operand_3, operand_1, 0, 0);
        }
        /* ---- Sound and music ---- */
        else if (opcode == EVENT_OPCODE_SOUND_EFFECT) {
            g_battle_sound_pending_sfx_id = first_halfword;
        } else if (opcode == EVENT_OPCODE_BG_SOUND) {
            operand_1 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_1, &battle_sound_bg_thread);
            battle_thread_set_parameters(operand_1, (s32)parameters, 0, 0);
        } else if (opcode == EVENT_OPCODE_EDIT_BG_SOUND) {
            operand_1 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_1, &battle_sound_edit_bg_thread);
            battle_thread_set_parameters(operand_1, (s32)parameters, 0, 0);
        } else if (opcode == EVENT_OPCODE_SWITCH_TRACK) {
            g_battle_music_switch_request = operand_1;
            g_battle_event_music_switch_volume = operand_2;
            g_battle_event_music_switch_fade = operand_3;
            battle_thread_yield();
        } else if (opcode == EVENT_OPCODE_FADE_SOUND) {
            /* Matching limitation: target sllv masks its count to five bits.
             * An explicit &31 adds an instruction with GCC 2.6.3, so counts
             * above 31 remain undefined in C; no script restriction is proven. */
            g_battle_music_volume_transition_request = operand_2 << (operand_1 + 0x10);
            battle_thread_yield();
        } else if (opcode == EVENT_OPCODE_END_TRACK) {
            g_battle_music_unload_slot_request = operand_1;
            battle_thread_yield();
        }
        /* ---- EVTCHR and reserved VRAM slots ---- */
        else if (opcode == EVENT_OPCODE_LOAD_EVTCHR) {
            operand_2 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_2, battle_gfx_load_evtchr_thread);
            battle_thread_set_parameters(operand_2, operand_1,
                battle_script_load_halfword(instruction_offset + ((u8*)g_battle_event_block) + 2), 0);
            battle_thread_yield();
        } else if (opcode == EVENT_OPCODE_LOAD_EVTCHR_CLEAR) {
            g_battle_event_pending_loaded_evtchr_clear_slot = operand_1;
            battle_thread_yield();
        } else if (opcode == EVENT_OPCODE_SAVE_EVTCHR) {
            g_battle_event_pending_evtchr_save_slot = operand_1;
            battle_thread_yield();
        } else if (opcode == EVENT_OPCODE_SAVE_EVTCHR_CLEAR) {
            g_battle_event_pending_saved_evtchr_clear_slot = operand_1;
            battle_thread_yield();
        } else if (opcode == EVENT_OPCODE_COPY_UNIT_GRAPHICS_TO_RESERVED_VRAM_SLOT) {
            g_battle_event_pending_unit_vram_copy = (battle_script_get_misc_id(first_halfword) << 8) | operand_3;
            battle_thread_yield();
        } else if (opcode == EVENT_OPCODE_RELEASE_RESERVED_VRAM_SLOT) {
            g_battle_event_pending_reserved_vram_release_slot = operand_1;
            battle_thread_yield();
        } else if (opcode == EVENT_OPCODE_UNKNOWN_E0) {
        }
        /* ---- Adding and removing units ---- */
        else if (opcode == EVENT_OPCODE_REMOVE_UNIT) {
            remove_unit_misc_id = battle_script_get_misc_id(first_halfword);
            if (remove_unit_misc_id != EVENT_MISC_ID_NONE) {
                work_index = remove_unit_misc_id;
                while (battle_unit_is_moving(work_index) != 0) {
                    battle_thread_yield();
                }
                if (work_index != EVENT_MISC_ID_NONE) {
                    operand_1 = battle_unit_get_battle_index_by_misc_id(work_index);
                    battle_unit_remove_misc_by_id(work_index);
                    if (operand_1 >= 0) {
                        battle_unit_set_cannot_exist(operand_1);
                    }
                }
            }
        } else if (opcode == EVENT_OPCODE_DRAW) {
            draw_misc_id = battle_script_get_misc_id(first_halfword);
            if (draw_misc_id != EVENT_MISC_ID_NONE) {
                battle_unit_draw_and_activate_by_misc_id(draw_misc_id);
                if (battle_script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT) == EVENT_ID_LIMBERRY_CEMETERY_BATTLE) {
                    battle_status_queue_current_status_graphics(
                        battle_unit_get_battle_index_by_misc_id(draw_misc_id), 1);
                    g_battle_thread_call_target = (void (*)(void))battle_unit_update_graphics_by_misc_id_wrapper;
                    battle_thread_call_on_main_stack(draw_misc_id);
                }
            }
        } else if (opcode == EVENT_OPCODE_ERASE) {
            reset_graphic_misc_id = battle_script_get_misc_id(first_halfword);
            if (reset_graphic_misc_id != EVENT_MISC_ID_NONE) {
                battle_gfx_reset_unit_graphic_trigger(reset_graphic_misc_id);
            }
        } else if (opcode == EVENT_OPCODE_ADD_GHOST_UNIT) {
            ghost_parameters = instruction_offset + ((u8*)g_battle_event_block) + 3;
            /* An empty four-pass countdown precedes the search for a free
             * misc slot. */
            ghost_misc_id = 3;
            do {
                ghost_misc_id -= 1;
            } while (ghost_misc_id >= 0);
            for (ghost_misc_id = 0; ghost_misc_id < EVENT_UNIT_SLOT_COUNT; ghost_misc_id++) {
                if (battle_unit_has_misc_id(ghost_misc_id) == 0) {
                    g_battle_event_unit_slots[ghost_parameters[0]] = ghost_misc_id;
                    break;
                }
            }
            if (ghost_misc_id != EVENT_UNIT_SLOT_COUNT) {
                battle_script_add_ghost_unit_event_instruction(ghost_parameters[1], ghost_parameters[2],
                    ghost_parameters[3], ghost_parameters[4], first_halfword,
                    g_battle_event_unit_slots[ghost_parameters[0]], ghost_parameters[5]);
                g_battle_unit_graphics_load_pending = 1U;
            } else {
                /* No free misc slot: the instruction is retried at once,
                 * with no yield in between. */
                continue;
            }
        } else if (opcode == EVENT_OPCODE_UNKNOWN_75) {
        } else if (opcode == EVENT_OPCODE_ADD_UNIT) {
            battle_script_add_unit_stub();
            added_unit = battle_unit_find_stats_by_entd_unit_id(first_halfword, &added_unit_state);
            if ((added_unit_state == -2) || (added_unit_state >= 0)) {
                for (added_battle_id = 0; added_battle_id < EVENT_UNIT_SLOT_COUNT; added_battle_id++) {
                    if (battle_unit_get_stats_from_battle_id(added_battle_id) == added_unit)
                        break;
                }
                operand_1 = battle_unit_get_misc_id_by_battle_id(added_battle_id);
                if (operand_1 == -1) {
                    battle_unit_save_staged_status_data(added_battle_id);
                    battle_gfx_load_unit_graphics_by_battle_id(added_battle_id, operand_3);
                    g_battle_unit_graphics_load_pending = 1U;
                } else {
                    if (operand_3 == 0) {
                        battle_unit_draw_and_activate_by_misc_id(operand_1);
                    }
                    if (battle_unit_save_staged_status_data(added_battle_id) != 0) {
                        g_battle_thread_call_target = (void (*)(void))battle_unit_update_graphics_by_misc_id_wrapper;
                        battle_thread_call_on_main_stack(operand_1);
                    }
                }
            }
        } else if (opcode == EVENT_OPCODE_WARP_UNIT) {
            battle_script_warp_unit((const event_warp_unit_parameters_t*)parameters);
        } else if (opcode == EVENT_OPCODE_UNKNOWN_61) {
        } else if (opcode == EVENT_OPCODE_SET_BATTLE_UNIT_POSITION) {
            /* Direct simulation slot, not the encoded identity used by WarpUnit.
             * Operand byte 1 is unused. The target neither guards slots >=21
             * nor synchronizes the renderer's position here. */
            unit = battle_unit_get_stats_from_battle_id(operand_1);
            position_instruction = instruction_offset + ((u8*)g_battle_event_block);
            unit->x = position_instruction[3];
            unit->position.bits.y = position_instruction[4];
            unit->position.bits.higher_elevation = position_instruction[5];
            unit->position.bits.facing = position_instruction[6];
        } else if (opcode == EVENT_OPCODE_WAIT_ADD_UNIT) {
            do {
                battle_thread_yield();
            } while (g_battle_unit_graphics_load_pending != 0);
        } else if (opcode == EVENT_OPCODE_WAIT_ADD_UNIT_END) {
            do {
                battle_thread_yield();
            } while (battle_thread_find_running_by_task(NATIVE_THREAD_TASK_ADD_UNIT) != 0);
        } else if (opcode == EVENT_OPCODE_ADD_UNIT_START) {
            operand_1 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_1, battle_script_add_unit_start_thread);
            g_battle_thread_contexts[operand_1].function_parameter_1 = (s32)parameters;
            instruction_offset
                = battle_script_find_instruction_byte_offset(instruction_offset, EVENT_OPCODE_ADD_UNIT_END);
        } else if (opcode == EVENT_OPCODE_ADD_UNIT_END) {
        }
        /* ---- Waits and synchronization ---- */
        else if (opcode == EVENT_OPCODE_WAIT_FOR_INSTRUCTION) {
            /* Yield until no thread runs the named task. The outer wait stays
             * a label loop: as a C loop its notes change the operand register
             * allocation (see the WORLD twin). */
            operand_3 = (operand_2 << 8) + operand_1;
            threads = g_battle_thread_contexts;
        wait_matching_task:
            battle_thread_yield();
            for (work_index = 1; work_index < 0x10; work_index++) {
                /* Typed indexing reverses the target addu operands; keep the 32-bit address sum. */
                thread = (native_thread_t*)((work_index * sizeof(native_thread_t)) + (u32)threads);
                if (thread->is_running != 0 && thread->task_id == operand_3) {
                    break;
                }
            }
            if (work_index != 0x10) {
                goto wait_matching_task;
            }
        } else if (opcode == EVENT_OPCODE_YIELD) {
            battle_thread_yield();
        } else if (opcode == EVENT_OPCODE_WAIT) {
            if (battle_script_is_tutorial_event_slot() != 0 && first_halfword >= 3) {
                battle_thread_wait_frames(first_halfword / battle_state_get_animation_speed());
            } else {
                battle_thread_wait_frames(first_halfword);
            }
        } else if (opcode == EVENT_OPCODE_TUTORIAL_BUTTON_WAIT) {
            do {
                battle_thread_yield();
            } while (!(first_halfword & g_battle_script_unfiltered_controller_input));
        } else if (opcode == EVENT_OPCODE_END_SOUND) {
            main_sound_stop_weather_sfx_music();
        }
        /* ---- Miscellaneous unit and display commands ---- */
        else if (opcode == EVENT_OPCODE_UNIT_SHADOW) {
            shadow_misc_id = battle_script_get_misc_id(first_halfword);
            if (shadow_misc_id != EVENT_MISC_ID_NONE) {
                if (operand_3 != 0) {
                    battle_gfx_clear_shadow_graphic_trigger_by_misc_id(shadow_misc_id);
                } else {
                    battle_gfx_set_shadow_graphic_trigger_by_misc_id(shadow_misc_id);
                }
            }
        } else if (opcode == EVENT_OPCODE_DEBUG_PAUSE) {
            battle_script_print_debug_message();
        } else if (opcode == EVENT_OPCODE_WAIT_VALUE) {
            battle_script_wait_value_event_instruction((const event_wait_value_parameters_t*)parameters);
        } else if (opcode == EVENT_OPCODE_EVTCHR_PALETTE) {
            evtchr_palette_misc_id = battle_script_get_misc_id(first_halfword);
            if (evtchr_palette_misc_id != EVENT_MISC_ID_NONE) {
                battle_gfx_set_evtchr_palette_by_misc_id(evtchr_palette_misc_id, operand_3, operand_4);
            }
        } else if (opcode == EVENT_OPCODE_LOAD_ATTACK_GRAPHICS) {
            battle_script_load_attack_graphics_event_instruction();
        } else if (opcode == EVENT_OPCODE_CHANGE_STATS) {
            battle_script_change_stats((const event_change_stats_parameters_t*)parameters);
        } else if (opcode == EVENT_OPCODE_PLAY_TUNE) {
            main_sound_play_tune(operand_1);
        } else if (opcode == EVENT_OPCODE_UNLOCK_DATE) {
            if ((battle_script_get_variable(EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS) == 0) && (operand_1 != 0)) {
                date_month = battle_script_get_variable(EVENT_SCRIPT_VAR_MONTH);
                battle_script_unlockdate_event_instruction(g_main_treasure_acquisition_date_bits, operand_1 - 1,
                    date_month, battle_script_get_variable(EVENT_SCRIPT_VAR_DAY));
            }
        } else if (opcode == EVENT_OPCODE_TEMP_WEAPON) {
            thrown_item_misc_id = battle_script_get_misc_id(first_halfword);
            if (thrown_item_misc_id != EVENT_MISC_ID_NONE) {
                battle_gfx_set_thrown_item_palette_by_misc_id(operand_3, thrown_item_misc_id);
            }
        } else if (opcode == EVENT_OPCODE_ARROW) {
            arrow_target_misc_id = battle_script_get_misc_id(first_halfword);
            if (arrow_target_misc_id != EVENT_MISC_ID_NONE) {
                operand_1 = battle_unit_get_battle_index_by_misc_id(arrow_target_misc_id);
                arrow_shooter_misc_id = battle_script_get_misc_id(
                    battle_script_load_halfword(instruction_offset + ((u8*)g_battle_event_block) + 3));
                if (arrow_shooter_misc_id != EVENT_MISC_ID_NONE) {
                    arrow_shooter_battle_id = battle_unit_get_battle_index_by_misc_id(arrow_shooter_misc_id);
                    g_battle_thread_call_target = (void (*)(void))battle_effect_set_arrow_trajectory;
                    battle_thread_call_on_main_stack(arrow_shooter_battle_id, g_battle_scratch_coords, operand_1);
                }
            }
        } else if (opcode == EVENT_OPCODE_MAP_FREEZE) {
            battle_map_freeze();
        } else if (opcode == EVENT_OPCODE_MAP_UNFREEZE) {
            battle_map_unfreeze();
        } else if (opcode == EVENT_OPCODE_UNIT_ANIM_ROTATE) {
            battle_script_unit_animation_rotate_event_instruction(
                (const event_unit_anim_rotate_parameters_t*)parameters);
        } else if (opcode == EVENT_OPCODE_WAIT_GRAPHIC_PRINT) {
            battle_thread_yield();
            do {
                battle_thread_yield();
                operand_1 = battle_thread_find_running_by_task(NATIVE_THREAD_TASK_SHOW_GRAPHIC);
            } while (operand_1 != 0 && g_battle_thread_contexts[operand_1].function_parameter_4 == 0);
        } else if (opcode == EVENT_OPCODE_LOCK_MENU_INPUT) {
            g_battle_menu_input_lock_mode = operand_1;
        } else if (opcode == EVENT_OPCODE_TUTORIAL_COORDINATE_CONFIRM) {
            while (g_battle_cursor_x != operand_1 || g_battle_cursor_y != operand_2
                || ((confirm_buttons = g_battle_script_unfiltered_controller_input) & PSX_PAD_CIRCLE) == 0
                || (confirm_buttons & (PSX_PAD_UP | PSX_PAD_RIGHT | PSX_PAD_DOWN | PSX_PAD_LEFT)) != 0) {
                battle_thread_yield();
            }
        }
        /* ---- Map title, dates, teleports, and status ---- */
        else if (opcode == EVENT_OPCODE_SHOW_MAP_TITLE) {
            operand_4 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_ATTACK);
            attack_map_load_title_graphic();
            battle_thread_start(operand_4, attack_map_show_title);
            battle_thread_set_parameters(operand_4, (s8)operand_1, (s8)operand_2, (s8)operand_3);
            battle_thread_wait_until_inactive(operand_4);
        } else if (opcode == EVENT_OPCODE_SET_DATE_ADVANCE) {
            if (battle_script_get_variable(EVENT_SCRIPT_VAR_SUPPRESS_PROGRESS_EFFECTS) == 0) {
                battle_script_set_variable(EVENT_SCRIPT_VAR_DATE_ADVANCE, first_halfword);
            }
        } else if (opcode == EVENT_OPCODE_TELEPORT_OUT) {
            battle_script_teleportout_event_instruction(first_halfword, 1);
        } else if (opcode == EVENT_OPCODE_TELEPORT_IN) {
            battle_script_teleportin_event_instruction(first_halfword, 1);
        } else if (opcode == EVENT_OPCODE_INFLICT_STATUS) {
            operand_1 = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
            battle_thread_start(operand_1, &battle_script_inflict_status_thread);
            battle_thread_set_parameters(operand_1, (s32)parameters, 0, 0);
        } else if (opcode == EVENT_OPCODE_UNKNOWN_95) {
        } else {
            break;
        }
        instruction_offset += 1 + g_battle_script_event_instruction_sizes[opcode];
    }
    battle_thread_exit_current();
}
