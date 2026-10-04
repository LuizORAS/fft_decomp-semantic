#include "fft/battle.h"
#include "psx/types.h"

/* Set g_battle_state_min_vsync_interval to value when it is 1-9; effect scripts use it to
 * slow their frames. */
void battle_state_set_min_vsync_interval(s32 value) {
    if ((u32)(value - 1) < 9) {
        g_battle_state_min_vsync_interval = value;
    }
}
