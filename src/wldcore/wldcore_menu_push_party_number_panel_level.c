#include "fft/wldcore.h"
#include "psx/pad.h"

/* Open the debug Party number panel (level PARTY_NUMBER_PANEL) that the world menu's debug row
 * requests: a number panel labeled "Party number:" (WLDCORE_TEXT_ID_PARTY_NUMBER_LABEL), 32-bit range value
 * 0x3e70000, starting at 0, run by the panel thread (WLDCORE_THREAD_SLOT_WINDOW_PANEL), with Circle and
 * Cross withheld from WORLD's menus. */
void wldcore_menu_push_party_number_panel_level(void) {
    open_point32_t origin;
    s32 record_index;

    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS;
    g_wldcore_menu_ordering_table_offset = 3;
    origin.x = -0x28;
    origin.y = -0x16;
    g_wldcore_active_menu_value = 0;
    wldcore_window_init_number_panel_render_thread(
        WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0x3E70000, WLDCORE_TEXT_ID_PARTY_NUMBER_LABEL, &origin);
    record_index = g_wldcore_menu_stack_depth;
    g_wldcore_menu_stack_records_next[record_index].panel.result = 0;
    g_wldcore_menu_stack_types[record_index + 1] = WLDCORE_MENU_LEVEL_PARTY_NUMBER_PANEL;
    g_wldcore_menu_stack_depth = record_index + 1;
}
