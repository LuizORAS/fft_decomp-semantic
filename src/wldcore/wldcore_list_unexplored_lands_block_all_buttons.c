#include "fft/wldcore.h"

/* Suspend handler of the Unexplored Land list: withhold every button from WORLD's menu threads while a level is
 * open over the list. wldcore_list_handle_unexplored_lands_input (and the Treasure list's step) calls it before
 * Select's help; the caller hands in its menu level, which the target does not read. */
void wldcore_list_unexplored_lands_block_all_buttons(wldcore_menu_panel_level_t* level) {
    g_wldcore_menu_blocked_buttons = -1;
}
