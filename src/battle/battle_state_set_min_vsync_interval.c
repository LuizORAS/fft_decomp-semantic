#include "fft/battle.h"
#include "psx/types.h"

/* Set g_main_system_min_vsync_interval, the fewest vertical blanks battle_state_sync_frame waits
 * per frame in the effect and action-execute states, to value when it is 1-9 (other values are
 * ignored); effect scripts use it to slow their frames. */
void battle_state_set_min_vsync_interval(s32 value) {
    if (value >= 1 && value <= 9) {
        g_main_system_min_vsync_interval = value;
    }
}
