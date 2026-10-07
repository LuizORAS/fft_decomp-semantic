#include "fft/battle.h"
#include "psx/types.h"

/* Clear g_battle_state_slowdown_frames; the effect stage calls it. */
void battle_state_clear_slowdown(void) {
    g_battle_state_slowdown_frames = 0;
}
