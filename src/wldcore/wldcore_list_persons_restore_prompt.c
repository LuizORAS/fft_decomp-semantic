#include "fft/wldcore.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Resume handler of the Persons list: block Circle, Cross and Select for the levels underneath and
 * show the Brave Story prompt (0xB848) again. */
void wldcore_list_persons_restore_prompt(void) {
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_BRAVE_STORY_INTRO, 0);
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
}
