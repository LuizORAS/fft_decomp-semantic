#include "fft/battle.h"
#include "psx/types.h"

/* Returns the hsync count battle_state_sync_frame measured for the last frame. No
 * code calls it. */
s32 battle_state_get_frame_measurement(void) {
    return g_main_system_battle_frame_hsync_count;
}
