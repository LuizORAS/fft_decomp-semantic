#include "fft/world.h"
#include "psx/types.h"

/* Build the list of available items (script variables 0x200..0x22a) and
 * position the menu around the currently selected one. `selected` stays
 * unset when no listed item is the current location. */
void world_menu_build_available_item_list(void) {
    s32 i;
    s32 count;
    s32 selected;
    world_menu_entry_t* menu;

    for (i = 0, count = 0; i < 0x2B; i++) {
        if (world_script_get_variable(i + 0x200) != 0) {
            g_world_menu_location_list_text_ids[count] = i - 0x7000;
            if (world_script_get_variable(i + 0x267) == 0) {
                g_world_menu_location_list_row_colors[count] = 8;
            } else {
                g_world_menu_location_list_row_colors[count] = 0;
            }
            if (i == world_script_get_variable(EVENT_SCRIPT_VAR_LOCATION)) {
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
    menu = g_world_menu_thread_menu_data;
    menu[6].selected_index = selected;
    menu[6].window_y = (7 - g_world_menu_location_list_layout.columns.row_count) * 8 - 0x48;
    world_menu_scrolling_list_thread();
}
