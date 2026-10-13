#include "fft/wldcore.h"

/* Block every button for the levels underneath while the battle tutorial help shows; the level is
 * not read. */
void wldcore_list_battle_tutorials_block_all_buttons(wldcore_menu_panel_level_t* unused_level) {
    g_wldcore_menu_blocked_buttons = -1;
}
