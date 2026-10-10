#include "fft/battle.h"

/* Enter the MapJumpingIn game state at two vertical blanks per frame. No code
 * calls it. */
void battle_state_halve_animation_speed_for_jump_in(void) {
    g_main_system_animation_speed = 2;
    g_battle_game_state = BATTLE_GAME_STATE_MAP_JUMPING_IN;
}
