#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/* Thread of WORLD's location list (g_menu_thread_menu_data entries 2 and 6, text 0x9000 + location):
 * list the locations whose visibility variable (EVENT_SCRIPT_VAR_LOCATION_VISIBLE_FIRST + location)
 * is set, in row color 4 for the current one, 8 for a red location and 0 for the others, then run
 * it as a scrolling list (world_menu_run_scrolling_ability_list_thread) of at most seven rows, scrolled to the
 * current location. `selected` stays unset when the current location is not listed. */
void world_map_destination_list_thread(void) {
    s32 i;
    s32 count;
    s32 selected;

    for (i = 0, count = 0; i < WORLD_LOCATION_COUNT; i++) {
        if (script_get_variable(i + EVENT_SCRIPT_VAR_LOCATION_VISIBLE_FIRST) != 0) {
            g_world_menu_location_list_text_ids[count] = i + (s16)TEXT_ID_LOCATION_NAME_BASE;
            if (script_get_variable(i + EVENT_SCRIPT_VAR_LOCATION_COLOR_FIRST) == 0) {
                g_world_menu_location_list_row_colors[count] = 8;
            } else {
                g_world_menu_location_list_row_colors[count] = 0;
            }
            if (i == script_get_variable(EVENT_SCRIPT_VAR_LOCATION)) {
                g_world_menu_location_list_row_colors[count] = 4;
                selected = count;
            }
            count++;
        }
    }
    if (count >= 8) {
        g_world_menu_location_list_layout.columns.row_count = 7;
        g_world_menu_location_list_layout.columns.hidden_rows = count - 7;
    } else {
        g_world_menu_location_list_layout.columns.row_count = count;
        g_world_menu_location_list_layout.columns.hidden_rows = 0;
    }
    if (g_world_menu_location_list_layout.columns.hidden_rows < selected) {
        g_world_menu_location_list_layout.row_offset = g_world_menu_location_list_layout.columns.hidden_rows;
    } else {
        g_world_menu_location_list_layout.row_offset = selected;
    }
    g_menu_thread_menu_data[6].selected_index = selected;
    g_menu_thread_menu_data[6].window.y = (7 - g_world_menu_location_list_layout.columns.row_count) * 8 - 0x48;
    world_menu_run_scrolling_ability_list_thread();
}
