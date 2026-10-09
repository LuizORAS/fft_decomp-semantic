#include "fft/battle.h"
#include "psx/types.h"

void battle_state_set_time_scale(s32 value) {
    if (value >= 1 && value <= 9) {
        g_frame_pacing = value;
    }
}
