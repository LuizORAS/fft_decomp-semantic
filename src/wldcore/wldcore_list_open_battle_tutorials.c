#include "fft/wldcore.h"
#include "psx/pad.h"

/* Open the battle tutorial list, a tutorial category: eight rows (entry values 0xc010 +
 * row) at (0x54, 0x50) with width 0x8c, a row marked 8 once its tutorial has been watched (bit 32 + row
 * of g_main_tutorial_progress_masks). Sibling of wldcore_list_open_formation_tutorials. */
void wldcore_list_open_battle_tutorials(void) {
    wldcore_point32_t dimensions;
    wldcore_point32_t origin;
    s32 i;
    s32 count;
    s32 record_index;

    for (i = 0; i < 8; i++) {
        g_wldcore_list_entry_values[i] = i - 0x3FF0;
        if (wldcore_test_bit(&g_main_tutorial_progress_masks[1], i) != 0) {
            g_wldcore_list_row_flags[i] = 8;
        } else {
            g_wldcore_list_row_flags[i] = 0;
        }
    }
    count = i;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].panel.entry_count = i;
    if (count < 8) {
        i = count;
    } else {
        i = 8;
    }
    dimensions.x = 0x8C;
    origin.x = 0x54;
    dimensions.y = i;
    origin.y = 0x50;
    wldcore_list_clamp_cursor_state(WLDCORE_CURSOR_SLOT_BATTLE_TUTORIALS, count, i);
    wldcore_window_start_panel_render_thread(WLDCORE_THREAD_SLOT_WINDOW_PANEL,
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].panel.entry_count, &dimensions, &origin);
    record_index = g_wldcore_menu_stack_depth;
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
    g_wldcore_menu_ordering_table_offset = 9;
    g_wldcore_menu_stack_records_next[record_index].panel.result = 0;
    g_wldcore_menu_stack_types[record_index + 1] = WLDCORE_MENU_LEVEL_BATTLE_TUTORIALS;
    g_wldcore_menu_stack_depth = record_index + 1;
}
