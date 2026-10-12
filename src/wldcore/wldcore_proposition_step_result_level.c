#include "fft/script.h"
#include "fft/wldcore.h"
#include "psx/pad.h"

/* Per-frame step of the proposition-result menu level.
 *
 * Phase 0 waits for confirm/cancel and then commits the result: it clears the
 * proposition's script flag bit 1 (and sets bit 4 on success), adds the reward
 * total g_wldcore_proposition_gil_amount to gil (saturating at 99,999,999), records the completion
 * date in the packed nine-bit arrays, releases each participant from the
 * proposition and pays them their share, marks the proposition
 * state byte, and finally compacts the participant record out of
 * g_main_active_propositions. Phase 1 waits for WORLD thread 14 and the audio
 * queue to drain and restarts the town music; phase 2 pops the level and hands
 * off to the countdown level or to wldcore_list_open_completed_propositions.
 *
 * The proposition, result and share 32-bit fields are read as g_wldcore_job_selection
 * members: the target keeps that block's address in a register, which GCC
 * only does for an aggregate view, not for the scalar g_wldcore_job_selection.proposition_index names. */
void wldcore_proposition_step_result_level(wldcore_proposition_result_level_t* level) {
    calendar_date_t date;
    s32 proposition_flags;
    s32 war_funds;
    s32 day;
    s32 month;
    s32 i;

    if (g_wldcore_window_render_records[level->render_index].flags & WORLD_RENDER_RECORD_POP_IN) {
        return;
    }
    if (level->phase == WLDCORE_PROPOSITION_RESULT_PHASE_DONE) {
        if (g_wldcore_audio_queue.count == 0 && g_wldcore_audio_queue.current_command == WLDCORE_AUDIO_COMMAND_NONE) {
            g_wldcore_menu_stack_depth = g_wldcore_menu_stack_depth - 1;
            if (wldcore_get_completion_milestone_rank() != 0) {
                wldcore_menu_push_countdown_level();
                return;
            }
            if (g_wldcore_proposition_report.recalled == 0) {
                wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_IN, 0x10);
            }
            wldcore_list_open_completed_propositions();
        }
        return;
    }
    if (level->phase == WLDCORE_PROPOSITION_RESULT_PHASE_CLOSING) {
        if (g_wldcore_audio_queue.count == 0 && g_wldcore_audio_queue.current_command == WLDCORE_AUDIO_COMMAND_NONE
            && world_thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE) == 0) {
            if (wldcore_get_completion_milestone_rank() == 0 && g_wldcore_proposition_report.recalled == 0) {
                wldcore_sound_enqueue_audio_command(
                    WLDCORE_AUDIO_COMMAND_PLAY_MUSIC, WLDCORE_AUDIO_MUSIC(2, MAIN_MUSIC_BAR));
            }
            level->phase = WLDCORE_PROPOSITION_RESULT_PHASE_DONE;
        }
        return;
    }

    if (level->delay != 0) {
        level->delay = level->delay - 1;
        if (level->delay == 0) {
            suzuki_sound_play_2_sfx(MAIN_SFX_PROPOSITION_SUCCESS_A, MAIN_SFX_PROPOSITION_SUCCESS_B);
        }
    }
    if (!(g_wldcore_new_button_presses & (PSX_PAD_CIRCLE | PSX_PAD_CROSS))) {
        return;
    }
    g_wldcore_window_render_record_count = g_wldcore_window_render_record_count - 1;
    g_wldcore_window_render_object_count = g_wldcore_window_render_object_count - 1;
    proposition_flags = script_get_variable(g_main_active_propositions[level->proposition].proposition_id
                            + EVENT_SCRIPT_VAR_PROPOSITION_FIRST)
        & 0xFFFD;
    if (g_wldcore_proposition_report.result == WLDCORE_PROPOSITION_RESULT_SUCCESS) {
        proposition_flags |= WLDCORE_PROPOSITION_VARIABLE_COMPLETED;
    }
    world_script_set_variable(
        g_main_active_propositions[level->proposition].proposition_id + EVENT_SCRIPT_VAR_PROPOSITION_FIRST,
        proposition_flags);
    war_funds = script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS) + g_wldcore_proposition_gil_amount;
    if (war_funds > 0x05F5E0FF) {
        war_funds = 0x05F5E0FF;
    }
    world_script_set_variable(EVENT_SCRIPT_VAR_WAR_FUNDS, war_funds);
    g_wldcore_displayed_numeric_value = script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS);
    day = script_get_variable(EVENT_SCRIPT_VAR_DAY);
    month = script_get_variable(EVENT_SCRIPT_VAR_MONTH);
    if (g_wldcore_proposition_report.result == WLDCORE_PROPOSITION_RESULT_SUCCESS) {
        date.day = day;
        date.month = month;
        wldcore_write_nine_bit_record(g_main_proposition_last_attempt_date_bits,
            g_main_active_propositions[level->proposition].proposition_id, &date);
    }
    if (g_wldcore_proposition_report.reward_type == WLDCORE_PROPOSITION_REWARD_TREASURE) {
        world_script_set_variable(g_wldcore_proposition_report.reward_index + EVENT_SCRIPT_VAR_TREASURE_FIRST, 1);
        date.day = day;
        date.month = month;
        wldcore_write_nine_bit_record(
            g_main_treasure_acquisition_date_bits, g_wldcore_proposition_report.reward_index, &date);
    }
    if (g_wldcore_proposition_report.reward_type == WLDCORE_PROPOSITION_REWARD_UNEXPLORED_LAND) {
        world_script_set_variable(
            g_wldcore_proposition_report.reward_index + EVENT_SCRIPT_VAR_UNEXPLORED_LAND_FIRST, 1);
        date.day = day;
        date.month = month;
        wldcore_write_nine_bit_record(
            g_main_land_discovery_date_bits, g_wldcore_proposition_report.reward_index, &date);
    }
    for (i = 0; i < g_main_active_propositions[g_wldcore_proposition_report.proposition_index].participant_count; i++) {
        wldcore_get_party_data_pointer(
            g_main_active_propositions[g_wldcore_proposition_report.proposition_index].participant_indices[i])
            ->proposition_status = 0;
        main_job_add_proposition_jp(
            g_main_active_propositions[g_wldcore_proposition_report.proposition_index].participant_indices[i],
            g_wldcore_proposition_report.participant_jp[i]);
    }
    if (g_wldcore_proposition_report.result == WLDCORE_PROPOSITION_RESULT_FAILURE) {
        g_main_proposition_states[g_main_active_propositions[g_wldcore_proposition_report.proposition_index]
                .proposition_id] = WLDCORE_PROPOSITION_STATE_FAILURE | 0x20;
    }
    if (g_wldcore_proposition_report.result == WLDCORE_PROPOSITION_RESULT_BAD_FAILURE) {
        g_main_proposition_states[g_main_active_propositions[g_wldcore_proposition_report.proposition_index]
                .proposition_id] = WLDCORE_PROPOSITION_STATE_BAD_FAILURE | 0x10;
    }
    for (i = level->proposition; i < g_main_save_proposition_count - 1; i++) {
        g_main_active_propositions[i] = g_main_active_propositions[i + 1];
    }
    g_main_save_proposition_count--;
    if (wldcore_get_completion_milestone_rank() == 0 && g_wldcore_proposition_report.recalled == 0) {
        wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_OUT, 0x10);
    }
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0, -1, 0);
    level->phase = WLDCORE_PROPOSITION_RESULT_PHASE_CLOSING;
}
