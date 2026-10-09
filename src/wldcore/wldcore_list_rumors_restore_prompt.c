#include "fft/wldcore.h"
#include "psx/pad.h"
#include "psx/types.h"

void wldcore_list_rumors_restore_prompt(void) {
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, 0xB808, 0);
}
