#include "fft/battle.h"
#include "psx/types.h"

/* Set the frame rate to 60 (1) or 30 (2) frames a second; returns -1, changing nothing, for
 * any other value. */
s32 battle_state_set_vsync_interval(s32 speed) {
    if ((u32)(speed - 1) < 2) {
        g_battle_state_vsync_interval = speed;
        return 0;
    }
    return -1;
}
