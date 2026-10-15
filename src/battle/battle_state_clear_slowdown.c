#include "fft/battle.h"
#include "psx/types.h"

/* Clear g_main_system_slowdown_frames, the slow-down the game loop starts when a frame of the
 * effect or action-execute state takes more than 0x200 hsyncs; the effect stage calls it. */
void battle_state_clear_slowdown(void) {
    g_main_system_slowdown_frames = 0;
}
