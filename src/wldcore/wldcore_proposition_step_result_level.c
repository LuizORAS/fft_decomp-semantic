#include "fft/script.h"
#include "fft/thread.h"
#include "fft/wldcore.h"
#include "psx/pad.h"

/* Step of the result panel. Circle or Cross settles the proposition: its variable loses
 * WLDCORE_PROPOSITION_VARIABLE_IN_PROGRESS (and gains _COMPLETED on a success), the total gil joins
 * the war funds (capped at 99,999,999), the dates of the success and of a treasure or land found are
 * recorded (wldcore_calendar_write_date), each unit comes home with its JP (main_job_add_proposition_jp),
 * a failure keeps the proposition off the Bar's list for 32 days and a bad failure for 16
 * (g_main_proposition_states), and the record leaves g_main_active_propositions. Once the message box
 * and the audio queue are done the Bar's music returns (not before a milestone, nor after a
 * call-back), then the level closes into the discovery milestone
 * (wldcore_proposition_push_milestone_level) or the list of job reports.
 *
 * The proposition, result and share fields are read as g_wldcore_proposition_outcome
 * members: the target keeps that block's address in a register, which GCC
 * only does for an aggregate view, not for the scalar g_wldcore_proposition_outcome.proposition_index names. */
void wldcore_proposition_step_result_level(wldcore_proposition_result_level_t* level) {
    calendar_date_t date;
    s32 proposition_flags;
    s32 war_funds;
    s32 day;
    s32 month;
    s32 i;

    if (g_wldcore_window_render_records[level->render_index].flags & WLDCORE_DISPLAY_OBJECT_POP_IN) {
        return;
    }
    if (level->phase == WLDCORE_PROPOSITION_RESULT_PHASE_DONE) {
        if (g_wldcore_audio_queue.count == 0 && g_wldcore_audio_queue.current_command == WLDCORE_AUDIO_COMMAND_NONE) {
            g_wldcore_menu_stack_depth = g_wldcore_menu_stack_depth - 1;
            if (wldcore_proposition_get_milestone_rank() != 0) {
                wldcore_proposition_push_milestone_level();
                return;
            }
            if (g_wldcore_proposition_outcome.recalled == 0) {
                wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_IN, 0x10);
            }
            wldcore_proposition_open_report_list();
        }
        return;
    }
    if (level->phase == WLDCORE_PROPOSITION_RESULT_PHASE_CLOSING) {
        if (g_wldcore_audio_queue.count == 0 && g_wldcore_audio_queue.current_command == WLDCORE_AUDIO_COMMAND_NONE
            && thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE) == 0) {
            if (wldcore_proposition_get_milestone_rank() == 0 && g_wldcore_proposition_outcome.recalled == 0) {
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
    if (!(g_wldcore_input_new_button_presses & (PSX_PAD_CIRCLE | PSX_PAD_CROSS))) {
        return;
    }
    g_wldcore_window_render_record_count = g_wldcore_window_render_record_count - 1;
    g_wldcore_window_render_object_count = g_wldcore_window_render_object_count - 1;
    proposition_flags = script_get_variable(g_main_active_propositions[level->proposition].proposition_id
                            + EVENT_SCRIPT_VAR_PROPOSITION_FIRST)
        & 0xFFFD;
    if (g_wldcore_proposition_outcome.result == WLDCORE_PROPOSITION_RESULT_SUCCESS) {
        proposition_flags |= WLDCORE_PROPOSITION_VARIABLE_COMPLETED;
    }
    script_set_variable(
        g_main_active_propositions[level->proposition].proposition_id + EVENT_SCRIPT_VAR_PROPOSITION_FIRST,
        proposition_flags);
    war_funds = script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS) + g_wldcore_proposition_gil_amount;
    if (war_funds > 0x05F5E0FF) {
        war_funds = 0x05F5E0FF;
    }
    script_set_variable(EVENT_SCRIPT_VAR_WAR_FUNDS, war_funds);
    g_wldcore_displayed_numeric_value = script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS);
    day = script_get_variable(EVENT_SCRIPT_VAR_DAY);
    month = script_get_variable(EVENT_SCRIPT_VAR_MONTH);
    if (g_wldcore_proposition_outcome.result == WLDCORE_PROPOSITION_RESULT_SUCCESS) {
        date.day = day;
        date.month = month;
        wldcore_calendar_write_date(g_main_proposition_last_attempt_date_bits,
            g_main_active_propositions[level->proposition].proposition_id, &date);
    }
    if (g_wldcore_proposition_outcome.reward_type == WLDCORE_PROPOSITION_REWARD_TREASURE) {
        script_set_variable(g_wldcore_proposition_outcome.reward_index + EVENT_SCRIPT_VAR_TREASURE_FIRST, 1);
        date.day = day;
        date.month = month;
        wldcore_calendar_write_date(
            g_main_treasure_acquisition_date_bits, g_wldcore_proposition_outcome.reward_index, &date);
    }
    if (g_wldcore_proposition_outcome.reward_type == WLDCORE_PROPOSITION_REWARD_UNEXPLORED_LAND) {
        script_set_variable(g_wldcore_proposition_outcome.reward_index + EVENT_SCRIPT_VAR_UNEXPLORED_LAND_FIRST, 1);
        date.day = day;
        date.month = month;
        wldcore_calendar_write_date(g_main_land_discovery_date_bits, g_wldcore_proposition_outcome.reward_index, &date);
    }
    for (i = 0; i < g_main_active_propositions[g_wldcore_proposition_outcome.proposition_index].participant_count;
        i++) {
        wldcore_unit_get_party_unit(
            g_main_active_propositions[g_wldcore_proposition_outcome.proposition_index].participant_indices[i])
            ->proposition_status = 0;
        main_job_add_proposition_jp(
            g_main_active_propositions[g_wldcore_proposition_outcome.proposition_index].participant_indices[i],
            g_wldcore_proposition_outcome.participant_jp[i]);
    }
    if (g_wldcore_proposition_outcome.result == WLDCORE_PROPOSITION_RESULT_FAILURE) {
        g_main_proposition_states[g_main_active_propositions[g_wldcore_proposition_outcome.proposition_index]
                .proposition_id] = WLDCORE_PROPOSITION_STATE_FAILURE | 0x20;
    }
    if (g_wldcore_proposition_outcome.result == WLDCORE_PROPOSITION_RESULT_BAD_FAILURE) {
        g_main_proposition_states[g_main_active_propositions[g_wldcore_proposition_outcome.proposition_index]
                .proposition_id] = WLDCORE_PROPOSITION_STATE_BAD_FAILURE | 0x10;
    }
    for (i = level->proposition; i < g_main_save_proposition_count - 1; i++) {
        g_main_active_propositions[i] = g_main_active_propositions[i + 1];
    }
    g_main_save_proposition_count--;
    if (wldcore_proposition_get_milestone_rank() == 0 && g_wldcore_proposition_outcome.recalled == 0) {
        wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_OUT, 0x10);
    }
    thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0, -1, 0);
    level->phase = WLDCORE_PROPOSITION_RESULT_PHASE_CLOSING;
}
