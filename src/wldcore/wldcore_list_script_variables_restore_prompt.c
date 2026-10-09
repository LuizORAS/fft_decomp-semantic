#include "fft/wldcore.h"
#include "psx/pad.h"
#include "psx/types.h"

void wldcore_list_script_variables_restore_prompt(void) {
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, 0xB848, 0);
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
}
