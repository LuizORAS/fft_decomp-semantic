#include "fft/wldcore.h"

/* Pushes the proposition-attempt detail text window as menu level 0x2a, the
 * partner of the 0x80088180 pop. Records the entry on the new level, decodes
 * that entry's last-attempt date out of the packed nine-bit array at
 * g_main_proposition_last_attempt_date_bits (dest[0] month 1-12, dest[1] day
 * 1-31), and sets up the shared scrollable text window at g_wldcore_scrollable_text_window for text
 * 0x8800 + entry with the month name (0xb84f + month) and the day as its two
 * text substitutions. Then hides the parent window-render level's window and
 * render record with flag 0x10 and starts WORLD thread 14 on text 0xb849.
 *
 * The coordinate mode is constant here, yet retail still switched on it to
 * choose the text image row (0x60 for mode 0, 0x70 for mode 1, as the sibling
 * setups store). The compiler drops the dead case only after its first CSE
 * pass, which is why the stores after image_y address their fields from the
 * text_id base register.
 *
 * `unused` reproduces eight bytes of locals below the date bytes that the
 * target never touches. */
void wldcore_menu_push_proposition_attempt_detail_level(s32 value) {
    s32 coordinate_mode = 1;
    u8 unused[8];
    u8 date[2];

    wldcore_wait_for_file_load();
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].window_index = value;
    wldcore_read_nine_bit_record(g_main_proposition_last_attempt_date_bits, value, date);
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
    g_wldcore_scrollable_text_window.text_width = 0xdc;
    g_wldcore_scrollable_text_window.rows_per_page = 8;
    g_wldcore_scrollable_text_window.priority = 8;
    g_wldcore_scrollable_text_window.image_coordinate_mode = coordinate_mode;
    g_wldcore_scrollable_text_window.extra_render_index = -1;
    g_wldcore_scrollable_text_window.text_substitutions[0] = date[0] + 0xb84f;
    g_wldcore_scrollable_text_window.text_substitutions[1] = date[1];
    wldcore_text_init_scrollable_window(&g_wldcore_scrollable_text_window);
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                 .window_render.window_index]
        .flags |= 0x10;
    g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                        .window_render.render_index]
        .flags |= 0x10;
    g_world_text_substitution_values[0] = value + 0x6000;
    world_thread_set_parameters(0xE, 0x19, 0xB849, 0);
    g_wldcore_menu_stack_types[g_wldcore_menu_stack_depth + 1] = WLDCORE_MENU_LEVEL_PROPOSITION_ATTEMPT_DETAIL;
    g_wldcore_menu_stack_depth = g_wldcore_menu_stack_depth + 1;
}
