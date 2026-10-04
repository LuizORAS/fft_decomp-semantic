#include "fft/battle.h"

/* Switch to 30 fps and the MAP_JUMPING_IN state. */
void battle_state_enter_map_jumping_in(void) {
    g_battle_state_vsync_interval = 2;
    g_battle_game_state = BATTLE_GAME_STATE_MAP_JUMPING_IN;
}
