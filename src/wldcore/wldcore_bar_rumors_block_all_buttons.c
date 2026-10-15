#include "fft/wldcore.h"
#include "psx/types.h"

/* Keep every button from the WORLD menu frame while the rumor list's help text is up.
 * The caller hands in its menu level; the target reads no argument. */
void wldcore_bar_rumors_block_all_buttons(wldcore_menu_panel_level_t* level) {
    g_wldcore_menu_blocked_buttons = -1;
}
