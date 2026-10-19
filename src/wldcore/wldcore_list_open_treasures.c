#include "fft/script.h"
#include "fft/thread.h"
#include "fft/wldcore.h"
#include "psx/pad.h"

/* Open the Brave Story's Treasure section (level TREASURES): one row per treasure found (the script
 * variables from EVENT_SCRIPT_VAR_TREASURE_FIRST, named by text TEXT_ID_TREASURE_NAME_BASE + index), up to
 * eight rows at (0x88, 0x50) with width 0x5a, with the saved cursor (slot TREASURES) clamped to the
 * list. Twin of wldcore_list_open_unexplored_lands. */
void wldcore_list_open_treasures(void) {
    wldcore_window_panel_layout_t layout;
    open_point32_t origin;
    s32 count;
    s32 i; /* reused as the visible-row count, which the target keeps in $s0 */
    s32 record_index;

    count = 0;
    for (i = 0; i < EVENT_SCRIPT_VAR_TREASURE_COUNT; i++) {
        if (script_get_variable(i + EVENT_SCRIPT_VAR_TREASURE_FIRST) != 0) {
            g_wldcore_list_entry_values[count] = i + (s16)TEXT_ID_TREASURE_NAME_BASE;
            g_wldcore_list_row_flags[count] = 0;
            count++;
        }
    }
    thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_BRAVE_STORY_INTRO, 0);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].panel.entry_count = count;
    if (count < 8) {
        i = count;
    } else {
        i = 8;
    }
    layout.text_width = 0x5A;
    origin.x = 0x88;
    layout.visible_rows = i;
    origin.y = 0x50;
    wldcore_list_clamp_cursor_state(WLDCORE_CURSOR_SLOT_TREASURES, count, i);
    wldcore_window_start_panel_render_thread(WLDCORE_THREAD_SLOT_WINDOW_PANEL,
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].panel.entry_count, &layout, &origin);
    record_index = g_wldcore_menu_stack_depth;
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
    g_wldcore_menu_stack_types[record_index + 1] = WLDCORE_MENU_LEVEL_TREASURES;
    g_wldcore_menu_stack_depth = record_index + 1;
}
