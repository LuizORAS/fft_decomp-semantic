#include "fft/thread.h"
#include "fft/wldcore.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Push the level that asks how many days the units are sent for
 * (WLDCORE_MENU_LEVEL_PROPOSITION_DAYS): the barkeeper names the row's minimum and maximum
 * (WLDCORE_TEXT_ID_CHOOSE_DAYS), and the number panel thread runs on that range from the minimum,
 * with Circle and Cross kept from the WORLD menu threads (the level's own step takes them). */
void wldcore_proposition_push_days_level(void) {
    open_point32_t origin;
    s32 record_index;
    s32 x;
    s32 y;

    x = g_wldcore_selected_proposition_row[0].min_days;
    y = g_wldcore_selected_proposition_row[0].max_days;
    g_text_substitution_values[0] = x;
    g_text_substitution_values[1] = y;
    thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_CHOOSE_DAYS, 0);
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS;
    g_wldcore_menu_ordering_table_offset = 8;
    origin.x = 0x38;
    origin.y = 0x20;
    g_wldcore_active_menu_value = x;
    wldcore_window_init_number_panel_render_thread(
        WLDCORE_THREAD_SLOT_WINDOW_PANEL, x | (y << 16), WLDCORE_TEXT_ID_DAYS_LABEL, &origin);
    record_index = g_wldcore_menu_stack_depth;
    g_wldcore_menu_stack_records_next[record_index].panel.result = 0;
    g_wldcore_menu_stack_types[record_index + 1] = WLDCORE_MENU_LEVEL_PROPOSITION_DAYS;
    g_wldcore_menu_stack_depth = record_index + 1;
}
