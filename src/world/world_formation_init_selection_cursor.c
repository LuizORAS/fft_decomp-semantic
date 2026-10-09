#include "fft/world.h"

/*
 * Initialize the selected formation unit's stat callbacks and cursor position.
 *
 * The scroll update must report 10 stationary iterations before the cursor
 * coordinates are finalized. The four-column layout uses signed coordinates.
 */
void world_formation_init_selection_cursor(s16 formation_index) {
    point16_t* cursor;
    s32 count;
    s16 x;

    world_menu_init_scrollable_list_core(0, 0, 0);
    g_world_menu_script_callbacks[0] = world_get_formation_unit_stat_by_mode;
    g_world_menu_script_callbacks[1] = world_get_formation_unit_stat_by_mode;
    g_world_menu_script_callbacks[2] = world_formation_get_unit_egg_hatch_progress;
    g_world_menu_script_callbacks[3] = world_formation_get_unit_egg_hatch_total;
    g_world_menu_script_callbacks[4] = world_formation_get_unit_level;
    g_world_menu_script_callbacks[5] = world_formation_get_unit_experience;
    g_world_menu_script_callbacks[6] = world_formation_get_unit_brave;
    g_world_menu_script_callbacks[7] = world_formation_get_unit_faith;
    g_world_formation_scroll_velocity = 0;
    D_801C833C = 0;
    g_world_formation_selected_unit_index = formation_index;
    count = 0;
    world_formation_stage_selected_unit();
    do {
        world_formation_step_scroll_velocity(&g_world_formation_scroll_velocity, &g_world_formation_scroll_position);
        if (g_world_formation_scroll_velocity == 0) {
            count++;
        }
    } while (count < 10);
    g_world_formation_scroll_enabled = world_formation_can_scroll_slots(g_world_formation_scroll_position);
    x = formation_index % 4;
    cursor = &g_world_formation_cursor_position;
    cursor->x = x * 62 + 34;
    /* x is reused for the scroll offset, read as a byte and then sign-extended. */
    x = (u8)g_world_formation_scroll_position;
    cursor->y = (s8)x + (formation_index >> 2) * 60 + 61;
    g_world_formation_selection_cursor_ready = 1;
    world_formation_reset_slots_and_frame_tpages();
}
