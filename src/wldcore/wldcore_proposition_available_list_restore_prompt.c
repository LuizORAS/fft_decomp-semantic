#include "fft/thread.h"
#include "fft/wldcore.h"
#include "psx/pad.h"

/* Resume handler of the list of propositions on offer: keep Circle, Cross and Select from the WORLD
 * menu frame and show its prompt (WLDCORE_TEXT_ID_PROPOSITIONS_INTRO) again. */
void wldcore_proposition_available_list_restore_prompt(void) {
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
    thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_PROPOSITIONS_INTRO, 0);
}
