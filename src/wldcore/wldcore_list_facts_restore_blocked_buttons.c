#include "fft/wldcore.h"
#include "psx/pad.h"

/* Resume handler of the Facts list: block Circle, Cross and Select for the levels underneath. */
void wldcore_list_facts_restore_blocked_buttons(void) {
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
}
