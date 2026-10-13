#include "fft/wldcore.h"

/* Suspend handler of the Treasure list: withhold every button from WORLD's menu threads while a level is
 * open over the list. Nothing calls it directly; the list's step uses the Unexplored Land list's
 * handler before Select's help. */
void wldcore_list_treasures_block_all_buttons(void) {
    g_wldcore_menu_blocked_buttons = -1;
}
