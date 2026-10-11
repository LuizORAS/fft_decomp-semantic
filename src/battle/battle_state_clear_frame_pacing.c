#include "fft/battle.h"
#include "psx/types.h"

/* Remove the minimum frame wait battle_state_set_time_scale set. */
void battle_state_clear_frame_pacing(void) {
    g_battle_frame_pacing = 0;
}
