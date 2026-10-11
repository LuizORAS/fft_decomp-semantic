#include "fft/battle.h"

/* Switch to 30 fps and the MAP_JUMPING_OUT state. Nothing calls it:
 * battle_state_start_map_jump_out enters that state itself. */
void battle_state_enter_map_jumping_out(void) {
    g_main_system_vsync_interval = 2;
    g_battle_game_state = BATTLE_GAME_STATE_MAP_JUMPING_OUT;
}
