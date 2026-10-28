#include "fft/menu.h"
#include "fft/wldcore.h"

/* Show a location's arrival text (a world help message; wldcore_menu_restore_level_on_world_entry) as
 * level SCROLLABLE_TEXT_WINDOW: the text is measured and the shared scrollable text window
 * (g_wldcore_scrollable_text_window) is sized around it (at most four rows) with a frame window, both
 * hidden until the step shows them; the parent level's window hides, and the location label and the
 * map's animation stop (MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL, MAIN_SYSTEM_FLAG_FREEZE_ANIMATION). */
void wldcore_menu_push_scrollable_text_window_level(s32 text) {
    s16 width;
    s16 rows;

    text_measure_pixels(&width, &rows, text_find_entry(text));
    if (rows > 4) {
        rows = 4;
    }

    {
        /* Sixteen frame bytes above the measured size that retail reserves
         * and never touches. */
        s32 unused[4];
        s32 index;
        s32 depth;

        index = wldcore_window_append_record_and_reset_color(
            g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].window_index = index;
        g_wldcore_window_records[index].sequence = WLDCORE_ANIM_SEQUENCE_CHOICE_PANEL_HEADER;
        g_wldcore_window_records[index].priority = 3;
        g_wldcore_window_records[index].anim_counter = 0;
        g_wldcore_window_records[index].frame_index = 0;
        g_wldcore_window_records[index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;

        g_wldcore_scrollable_text_window.text_id = text;
        g_wldcore_scrollable_text_window.image_x = 0;
        g_wldcore_scrollable_text_window.image_y = 0x60;
        g_wldcore_scrollable_text_window.base.x = -(((width + 24) & 0xFFFC) >> 1);
        g_wldcore_scrollable_text_window.base.y = -((rows << 3) + 8);
        g_wldcore_scrollable_text_window.text_width = width;
        g_wldcore_scrollable_text_window.rows_per_page = rows;
        g_wldcore_scrollable_text_window.priority = 3;
        g_wldcore_scrollable_text_window.image_coordinate_mode = 0;
        g_wldcore_scrollable_text_window.extra_render_index = -1;
        g_wldcore_scrollable_text_window.text_substitutions[1] = -1;
        g_wldcore_scrollable_text_window.text_substitutions[0] = -1;
        wldcore_text_init_scrollable_window(&g_wldcore_scrollable_text_window);

        g_wldcore_window_render_records[g_wldcore_scrollable_text_window.render_record_index].flags
            |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        depth = g_wldcore_menu_stack_depth;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].window_index].position.x
            = g_wldcore_scrollable_text_window.base.x + 3;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].window_index].position.y
            = g_wldcore_scrollable_text_window.base.y - 2;
        g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL | MAIN_SYSTEM_FLAG_FREEZE_ANIMATION;
        g_wldcore_menu_stack_depth = depth + 1;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 1].window_index].flags
            |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_menu_stack_records_next[depth].scrollable_text._unknown_20 = 0x28;
        g_wldcore_menu_stack_types[depth + 1] = WLDCORE_MENU_LEVEL_SCROLLABLE_TEXT_WINDOW;
    }
}
