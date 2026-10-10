#include "fft/battle.h"
#include "psx/types.h"

/* End free map panning: the camera follows the cursor tile again. */
void battle_state_stop_game_flow(void) {
    g_battle_state_game_flow_running = 0;
}
