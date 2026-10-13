#include "fft/wldcore.h"

/* Block every button for the levels underneath while the Persons help shows. The caller hands in
 * its menu level; the target reads no argument. */
void wldcore_list_persons_block_all_buttons(wldcore_menu_panel_level_t* level) {
    g_wldcore_menu_blocked_buttons = -1;
}
