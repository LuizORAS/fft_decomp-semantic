#include "fft/wldcore.h"

/* Opens the selected rumor in a scrollable text window and hides the parent
 * level's two windows and render record until the detail level closes.
 *
 * The coordinate mode is constant here, yet retail still switched on it to
 * choose the text image row (0x60 for mode 0, 0x70 for mode 1, as the sibling
 * setups store). The compiler drops the dead case only after its first CSE
 * pass, which is why the stores after image_y address their fields from the
 * text_id base register.
 *
 * `unused` retains the target's 8 reserved frame bytes. */
void wldcore_bar_push_rumor_detail_level(s32 value) {
    s32 coordinate_mode = 0;
    s32 depth;
    u8 unused[8];

    g_world_text_substitution_values[0] = value + 0xC800;
    world_thread_set_parameters(0xE, 0x19, 0xB809, 0);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].rumor_detail.rumor_index = value;
    g_wldcore_scrollable_text_window.text_id = value + 0x8800;
    g_wldcore_scrollable_text_window.image_x = 0;
    switch (coordinate_mode) {
    case 0:
        g_wldcore_scrollable_text_window.image_y = 0x60;
        break;
    case 1:
        g_wldcore_scrollable_text_window.image_y = 0x70;
        break;
    }
    g_wldcore_scrollable_text_window.base.x = -122;
    g_wldcore_scrollable_text_window.base.y = -40;
    g_wldcore_scrollable_text_window.text_width = 0xDC;
    g_wldcore_scrollable_text_window.rows_per_page = 8;
    g_wldcore_scrollable_text_window.priority = 9;
    g_wldcore_scrollable_text_window.image_coordinate_mode = coordinate_mode;
    g_wldcore_scrollable_text_window.extra_render_index = -1;
    g_wldcore_scrollable_text_window.text_substitutions[1] = -1;
    g_wldcore_scrollable_text_window.text_substitutions[0] = -1;
    wldcore_text_init_scrollable_window(&g_wldcore_scrollable_text_window);
    depth = g_wldcore_menu_stack_depth;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.first_window].flags
        |= 0x10;
    g_wldcore_menu_stack_records_next[depth].rumor_detail.first_window
        = g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.first_window;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.second_window].flags
        |= 0x10;
    g_wldcore_menu_stack_records_next[depth].rumor_detail.second_window
        = g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.second_window;
    g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.render_index].flags
        |= 0x10;
    g_wldcore_menu_stack_depth = depth + 1;
    g_wldcore_menu_stack_records_next[depth].rumor_detail.render_index
        = g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.render_index;
    g_wldcore_menu_stack_types[depth + 1] = WLDCORE_MENU_LEVEL_RUMOR_DETAIL;
}
