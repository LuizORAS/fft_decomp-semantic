#include "fft/wldcore.h"
#include "psx/types.h"

/* Pop the rumor list's level: the menu ordering table offset back to 1,
 * MAIN_SYSTEM_FLAG_INPUT_BLOCKED toggled off and the buttons unblocked.
 * The caller hands in its menu level; the target reads no argument. */
void wldcore_bar_rumors_close_level(wldcore_menu_panel_level_t* level) {
    g_wldcore_menu_ordering_table_offset = 1;
    g_main_system_flags ^= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
    --g_wldcore_menu_stack_depth;
    g_wldcore_menu_blocked_buttons = 0;
}
