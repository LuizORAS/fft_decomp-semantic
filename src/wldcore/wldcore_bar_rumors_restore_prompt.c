#include "fft/wldcore.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Resume handler of the rumor list: keep Circle, Cross and Select from the WORLD menu frame and
 * show the rumors prompt (WLDCORE_TEXT_ID_RUMORS_INTRO) again. */
void wldcore_bar_rumors_restore_prompt(void) {
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_RUMORS_INTRO, 0);
}
