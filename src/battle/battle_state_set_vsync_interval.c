#include "fft/battle.h"
#include "psx/types.h"

/* Set the frame rate to 60 (1) or 30 (2) frames a second (g_main_system_vsync_interval) and
 * return 0; any other value returns -1 and changes nothing. */
s32 battle_state_set_vsync_interval(s32 speed) {
    if (speed == 1 || speed == 2) {
        g_main_system_vsync_interval = speed;
        return 0;
    }
    return -1;
}
