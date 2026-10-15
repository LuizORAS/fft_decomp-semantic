#include "fft/wldcore.h"
#include "psx/types.h"

/* Resume handler of a participant's status panels, once the status help closes: the L1 and R1 prompt
 * windows go back to CLUT 0. */
void wldcore_menu_resume_participant_detail_level(s32* param) {
    s32 first = param[1];
    s32 second = param[0];

    g_wldcore_window_records[first].palette = 0;
    g_wldcore_window_records[second].palette = 0;
}
