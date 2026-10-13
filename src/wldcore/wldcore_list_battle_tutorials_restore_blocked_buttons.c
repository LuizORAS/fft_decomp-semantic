#include "fft/wldcore.h"
#include "psx/pad.h"

/* Resume handler of the battle tutorial list: withhold Circle, Cross and Select from WORLD's menu threads again, so
 * only the list's own input step reacts to them. */
void wldcore_list_battle_tutorials_restore_blocked_buttons(void) {
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
}
