#include "fft/thread.h"
#include "fft/wldcore.h"
#include "psx/pad.h"

/* Resume handler of the Unexplored Land list: withhold Circle, Cross and Select from WORLD's menu threads again, so
 * only the list's own input step reacts to them, and show the Brave Story's prompt (WLDCORE_TEXT_ID_BRAVE_STORY_INTRO)
 * again. */
void wldcore_list_unexplored_lands_restore_prompt(void) {
    g_wldcore_menu_blocked_buttons = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
    thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_BRAVE_STORY_INTRO, 0);
}
