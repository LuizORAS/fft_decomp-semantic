#include "fft/wldcore.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Resume handler of the list of propositions in progress: keep Circle, Cross and Select from the
 * WORLD menu frame and show its line (WLDCORE_TEXT_ID_JOBS_IN_PROGRESS) again. */
void wldcore_proposition_active_list_restore_prompt(void) {
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_JOBS_IN_PROGRESS, 0);
}
