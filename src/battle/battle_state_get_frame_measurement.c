#include "fft/battle.h"
#include "psx/types.h"

/* Return the last frame's VSync timer-1 count. */
s32 battle_state_get_frame_measurement(void) {
    return g_battle_frame_measurement;
}
