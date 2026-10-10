#include "fft/battle.h"
#include "psx/types.h"

/* Set g_frame_pacing, the minimum number of vertical blanks
 * battle_state_sync_frame waits per frame in the effect and action-execute
 * states. Values outside 1-9 are ignored. */
void battle_state_set_time_scale(s32 value) {
    if (value >= 1 && value <= 9) {
        g_frame_pacing = value;
    }
}
