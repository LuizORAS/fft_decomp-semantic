#include "fft/open.h"
#include "psx/types.h"

/* Fills the music-test list's column layout and menu entry, then starts the
 * AT list thread on them. The window is the extent's width plus a 24-pixel
 * margin, rounded down to a multiple of four; its height fields start at 0. */
void open_menu_start_music_test_list_thread(s32 thread_id, s32 limit, open_point32_t* extent, open_point32_t* origin) {
    s32 width;

    width = (extent->x + 0x18) & 0xFFFC;
    g_open_music_test_list_state.row_count = extent->y;
    g_open_music_test_list_state.hidden_rows = limit - extent->y;
    g_open_music_test_list_state.x[1] = 4;
    g_open_music_test_list_state.x[0] = 4;
    g_open_music_test_list_state.mode[1] = 2;
    g_open_music_test_list_state.mode[2] = 2;
    g_open_music_test_list_state.text_ids[0] = g_open_music_test_list_entries;
    g_open_music_test_list_state.text_colors[0] = g_open_music_test_list_entry_flags;
    g_open_music_test_list_state.mode[0] = 0;
    g_open_music_test_list_params.vram_x = 0x200;
    g_open_music_test_list_params.vram_y = 0x100;
    g_open_music_test_list_params.inner_width = width;
    g_open_music_test_list_params.inner_height = 0;
    g_open_music_test_list_params.window_x = origin->x - 0x80;
    g_open_music_test_list_params.window_y = origin->y - 0x78;
    g_open_music_test_list_params.window_width = width;
    g_open_music_test_list_params.window_height = 0;
    g_open_music_test_list_params.text_image.x_load = 0;
    g_open_music_test_list_params.text_image.y_load = 0;
    g_open_music_test_list_params.text_image.width = width;
    g_open_music_test_list_params.text_image.height = 0;
    g_open_music_test_list_params.text_image.x_screen_offset = 0;
    g_open_music_test_list_params.text_image.y_screen_offset = 0;
    g_open_music_test_list_params.text_id = 0;
    g_open_music_test_list_params.max_row_index = extent->y;
    g_open_music_test_list_params.thread_count = 0;
    g_open_music_test_list_params.parent_indices = g_open_music_test_row_actions;
    g_open_music_test_list_params.header_id = 1;
    /* The list thread reads text_binding as a column layout, not a binding. */
    g_open_music_test_list_params.text_binding = (struct world_menu_text_binding*)&g_open_music_test_list_state;
    g_open_music_test_list_params.value = &g_open_music_test_selected_option;
    g_open_music_test_list_params.selected_index = 0;
    g_open_music_test_list_params.select_text_table = 0;

    world_thread_start(thread_id, world_build_at_list_2);
    world_thread_set_parameters(thread_id, (s32)&g_open_music_test_list_params, 0, 0);
}
