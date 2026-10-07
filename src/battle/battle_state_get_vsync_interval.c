#include "fft/battle.h"
#include "psx/types.h"

/* Return g_battle_state_vsync_interval. */
s32 battle_state_get_vsync_interval(void) {
    return g_battle_state_vsync_interval;
}
