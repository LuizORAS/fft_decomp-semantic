#include "fft/wldcore.h"
#include "psx/types.h"

/* Resume handler of a participant's status panels, once the status help closes: the L1 and R1 prompt
 * windows go back to CLUT 0. */
void wldcore_menu_resume_participant_detail_level(wldcore_menu_stack_record_t* level) {
    s32 second = level->participant.left_window;

    g_wldcore_window_records[level->participant.right_window].palette = 0;
    g_wldcore_window_records[second].palette = 0;
}
