#include "fft/battle.h"
#include "psx/types.h"

s32 battle_state_get_frame_measurement(void) {
    return g_main_system_battle_frame_hsync_count;
}
