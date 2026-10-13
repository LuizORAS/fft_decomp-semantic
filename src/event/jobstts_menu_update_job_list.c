#include "fft/event_jobstts.h"
#include "psx/pad.h"

/* Run the job list menu; on Circle, record the chosen job and return 1 to advance the menu state. */
s32 jobstts_menu_update_job_list(void) {
    s16 state[10];
    s32 input;

    if (g_jobstts_job_list_menu_initialized == 0) {
        g_jobstts_menu_script_conditions[0] = jobstts_job_calculate_current_level;
        g_jobstts_menu_script_conditions[2] = jobstts_job_calculate_current_jp;
        g_jobstts_menu_script_conditions[5] = jobstts_job_calculate_next_level_jp_requirement;
        g_jobstts_menu_script_conditions[6] = jobstts_job_calculate_current_total_jp;
        g_jobstts_menu_script_conditions[13] = jobstts_job_calculate_current_mastered;
        jobstts_job_build_unit_job_list(0, g_jobstts_job_ids, 0);
        jobstts_menu_get_selection_record(4, &state[8], &state[9], g_jobstts_job_ids);
        jobstts_menu_init_scrollable_list(
            g_jobstts_job_ids, state[8], state[9], g_battle_text_section_pointers[TEXT_SECTION_JOB_NAMES]);
        g_jobstts_job_list_menu_initialized = 1;
    }

    g_jobstts_text_selected_job_id = g_jobstts_job_ids[g_jobstts_menu_list_selected_index] + 0x6000;
    input = g_jobstts_input_primary_repeat;
    if (input & PSX_PAD_CIRCLE) {
        g_jobstts_job_selected_id = g_jobstts_job_ids[g_jobstts_menu_list_selected_index];
        jobstts_job_calculate_current_level(g_jobstts_menu_list_selected_index);
        jobstts_job_calculate_current_jp(g_jobstts_menu_list_selected_index);
        jobstts_job_calculate_current_total_jp(g_jobstts_menu_list_selected_index);
        if (jobstts_job_calculate_current_mastered(g_jobstts_menu_list_selected_index) == 0) {
            jobstts_job_calculate_next_level_jp_requirement(g_jobstts_menu_list_selected_index);
        }
        jobstts_menu_set_selection_record(
            4, g_jobstts_menu_list_selected_index, g_jobstts_menu_scroll_base_index, g_jobstts_job_ids);
        g_jobstts_job_list_menu_initialized = 0;
        g_jobstts_sound_queued_effect_id = MAIN_SFX_CONFIRM;
        return MENU_JOB_LIST_STEP_ABILITIES;
    }
    jobstts_menu_run_display_script_with_mode(g_jobstts_job_list_render_commands, input, g_battle_menu_input_lock_mode);
    return MENU_JOB_LIST_STEP_JOBS;
}
