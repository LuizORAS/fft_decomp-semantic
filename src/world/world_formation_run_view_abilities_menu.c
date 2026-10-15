#include "fft/world.h"
#include "psx/types.h"

/* Handler of the formation's View Abilities (VIEW_ABILITIES in g_world_formation_menu_handlers,
 * Circle on an open unit): the job list (world_formation_run_ability_job_list), then the chosen job's
 * abilities (world_ability_run_view_list), then back to the unit grid. */
void world_formation_run_view_abilities_menu(void) {
    if (g_world_view_abilities_initialized == 0) {
        g_world_view_abilities_initialized = 1;
        g_world_formation_unit_cycle_mode = 1;
        g_world_view_abilities_step = MENU_JOB_LIST_STEP_JOBS;
        g_world_status_display_thread_params.style = 1;
        g_world_numeric_editor_thread_params.style = 1;
        g_world_equipment_ability_panel_thread_params.style = 1;
        world_menu_init_cursor_position(WORLD_MENU_CURSOR_ABILITY_TAB, 0);
        world_menu_clear_selection_records();
        g_world_menu_sound_effect_id = MAIN_SFX_CONFIRM;
    }
    if (g_world_view_abilities_step == MENU_JOB_LIST_STEP_JOBS) {
        g_world_view_abilities_step = world_formation_run_ability_job_list();
    } else if (g_world_view_abilities_step == MENU_JOB_LIST_STEP_ABILITIES) {
        g_world_view_abilities_step = world_ability_run_view_list();
    }
    if (g_world_view_abilities_step == MENU_JOB_LIST_STEP_CLOSE) {
        g_world_numeric_editor_thread_params.style = 0;
        g_world_equipment_ability_panel_thread_params.style = 0;
        g_world_formation_current_menu = WORLD_FORMATION_MENU_UNIT_GRID;
        g_world_formation_unit_cycle_mode = 2;
        world_input_clear_state();
        g_world_input_secondary_repeat_latched = 0;
        g_world_status_display_thread_params.style = 0;
        g_world_view_abilities_initialized = 0;
    }
}
