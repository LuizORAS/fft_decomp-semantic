#include "fft/battle.h"

/* Stop the slow-down timer the game loop starts when a frame of the effect or
 * action-execute state takes more than 0x200 hsyncs. */
void battle_state_reset_frame_pacing_timer(void) {
    g_battle_frame_pacing_timer = 0;
}
