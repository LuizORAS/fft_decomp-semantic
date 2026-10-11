#include "fft/battle.h"
#include "psx/types.h"

/* Return the last frame's hsync count (g_main_system_battle_frame_hsync_count, the VSync result
 * battle_state_sync_frame returned). Nothing calls it. */
s32 battle_state_get_frame_measurement(void) {
    return g_main_system_battle_frame_hsync_count;
}
