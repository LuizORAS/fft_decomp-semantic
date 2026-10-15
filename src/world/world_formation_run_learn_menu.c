#include "fft/world.h"
#include "psx/types.h"

/* The formation's Learn menu: the unit's job list (world_formation_run_ability_job_list), then the
 * chosen job's ability list (world_ability_run_learn_list). Closing the job list saves the roster
 * and returns 0. */
s32 world_formation_run_learn_menu(void) {
    s32 result;

    if (g_world_learn_menu_initialized == 0) {
        g_world_learn_menu_step = MENU_JOB_LIST_STEP_JOBS;
        g_world_formation_unit_banner_enabled = 0;
        world_menu_request_thread_stop(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR);
        world_menu_request_thread_stop(WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL);
        g_world_formation_unit_cycle_mode = 0;
        world_menu_init_cursor_position(WORLD_MENU_CURSOR_ABILITY_TAB, 0);
        world_menu_clear_selection_record(9);
        g_world_learn_menu_initialized = 1;
    }
    if (g_world_learn_menu_step == MENU_JOB_LIST_STEP_JOBS) {
        result = world_formation_run_ability_job_list();
        if (result == MENU_JOB_LIST_STEP_ABILITIES) {
            world_menu_init_scrollable_list_core(0, 0, 0);
            g_world_learn_menu_step = MENU_JOB_LIST_STEP_ABILITIES;
        } else if (result == MENU_JOB_LIST_STEP_CLOSE) {
            g_world_learn_menu_initialized = 0;
            g_world_formation_unit_banner_enabled = 1;
            world_formation_save_records_to_party_data();
            return 0;
        }
    } else {
        g_world_learn_menu_step = world_ability_run_learn_list();
    }
    return 1;
}
