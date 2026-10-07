#include "fft/battle.h"
#include "psx/types.h"

/* Clear g_battle_state_min_vsync_interval; the effect stage calls it. */
void battle_state_clear_min_vsync_interval(void) {
    g_battle_state_min_vsync_interval = 0;
}
