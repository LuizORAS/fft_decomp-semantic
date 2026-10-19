#include "fft/battle.h"

/* Switch to 30 fps and the MAP_JUMPING_IN state. Nothing calls it:
 * battle_state_handle_map_init_state enters that state itself. */
void battle_state_enter_map_jumping_in(void) {
    g_main_system_vsync_interval = 2;
    g_battle_game_state = BATTLE_GAME_STATE_MAP_JUMPING_IN;
}
