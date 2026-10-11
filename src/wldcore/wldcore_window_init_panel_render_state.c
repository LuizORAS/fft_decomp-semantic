#include "fft/wldcore.h"

/* Initialize the panel renderer's shared geometry and data pointers.
 *
 * Width is rounded for its tiled image; stored origins are screen-relative.
 * Each coordinate is read into an int before it is narrowed into its s16
 * field: the target loads full words. */
void wldcore_window_init_panel_render_state(s32 unused, s32 extent, point32_t* dimensions, point32_t* origin) {
    s32 width = dimensions->x;
    s32 rows = dimensions->y;
    s32 visible_rows;
    s32 text_width;
    s32 x;
    s32 y;
    s32 last_row;

    g_wldcore_window_panel_label_state.row_count = rows;
    visible_rows = dimensions->y;
    g_wldcore_window_panel_label_state.hidden_rows = extent - visible_rows;
    g_wldcore_window_panel_label_state.x[0] = g_wldcore_window_panel_label_state.x[1] = 4;
    g_wldcore_window_panel_label_state.mode[0] = MENU_TEXT_COLUMN_TEXT;
    g_wldcore_window_panel_label_state.mode[1] = MENU_TEXT_COLUMN_HIDDEN;
    g_wldcore_window_panel_label_state.mode[2] = MENU_TEXT_COLUMN_HIDDEN;
    g_wldcore_window_panel_label_state.text_ids[0] = g_wldcore_list_entry_values;
    g_wldcore_window_panel_label_state.text_colors[0] = g_wldcore_list_row_flags;
    text_width = (width + 0x18) & 0xFFFC;
    g_wldcore_window_panel_render_state.vram_origin.x = 0x200;
    g_wldcore_window_panel_render_state.vram_origin.y = 0x100;
    g_wldcore_window_panel_render_state.inner_width = text_width;
    g_wldcore_window_panel_render_state.inner_height = 0;
    x = origin->x;
    g_wldcore_window_panel_render_state.window_x = x - 0x80;
    y = origin->y;
    g_wldcore_window_panel_render_state.window_y = y - 0x78;
    g_wldcore_window_panel_render_state.window_width = text_width;
    g_wldcore_window_panel_render_state.window_height = 0;
    g_wldcore_window_panel_render_state.text_image.x_load = 0;
    g_wldcore_window_panel_render_state.text_image.y_load = 0;
    g_wldcore_window_panel_render_state.text_image.width = text_width;
    g_wldcore_window_panel_render_state.text_image.height = 0;
    g_wldcore_window_panel_render_state.text_image.x_screen_offset = 0;
    g_wldcore_window_panel_render_state.text_image.y_screen_offset = 0;
    g_wldcore_window_panel_render_state.text_id = 0;
    last_row = dimensions->y;
    g_wldcore_window_panel_render_state.max_row_index = last_row;
    g_wldcore_window_panel_render_state.thread_count = 0;
    g_wldcore_window_panel_render_state.row_actions = g_wldcore_panel_row_actions;
    g_wldcore_window_panel_render_state.header_id = 1;
    g_wldcore_window_panel_render_state.context = &g_wldcore_window_panel_label_state; /* a column layout */
    g_wldcore_window_panel_render_state.value = &g_wldcore_active_menu_value;
}
