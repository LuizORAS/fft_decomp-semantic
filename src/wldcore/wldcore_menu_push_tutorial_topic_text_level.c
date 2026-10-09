#include "fft/wldcore.h"

/* Pushes the tutorial topic text window as menu level 0x22: records the topic
 * id on the new level, sets up the shared scrollable text window at g_wldcore_scrollable_text_window
 * for that topic's text (0x8800 + topic), and hides the parent pair-render
 * level's two windows and its render record with flag 0x10. wldcore_menu_wait_tutorial_topic_text_then_pop_level
 * steps the window and pops the level, restoring all three.
 *
 * The coordinate mode is constant here, yet retail still switched on it to
 * choose the text image row (0x60 for mode 0, 0x70 for mode 1, as the sibling
 * setups store). The compiler drops the dead case only after its first CSE
 * pass, which is why the stores after image_y address their fields from the
 * text_id base register.
 *
 * The menu-stack type store comes last so the
 * compiler scales depth by four late and in place, forcing the copy into v0
 * for depth + 1. */
void wldcore_menu_push_tutorial_topic_text_level(s32 topic) {
    s32 coordinate_mode = 1;
    s32 depth;

    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].window_index = topic;
    g_wldcore_scrollable_text_window.text_id = topic + 0x8800;
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
    g_wldcore_scrollable_text_window.priority = 9;
    g_wldcore_scrollable_text_window.image_coordinate_mode = coordinate_mode;
    g_wldcore_scrollable_text_window.extra_render_index = -1;
    g_wldcore_scrollable_text_window.text_substitutions[1] = -1;
    g_wldcore_scrollable_text_window.text_substitutions[0] = -1;
    wldcore_text_init_scrollable_window(&g_wldcore_scrollable_text_window);
    depth = g_wldcore_menu_stack_depth;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.first_window].flags
        |= 0x10;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.second_window].flags
        |= 0x10;
    g_wldcore_menu_stack_depth = depth + 1;
    g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.render_index].flags
        |= 0x10;
    g_wldcore_menu_stack_types_next[depth] = WLDCORE_MENU_LEVEL_TUTORIAL_TOPIC_TEXT;
}
