#include "fft/battle.h"

/* Fade the screen back in after battle_state_start_change_map_jump_out (MAP_JUMPING_IN_2, saving
 * the current state to return to), 0x100 / duration a frame (one frame for a duration of 0),
 * running the event script meanwhile. */
void battle_state_start_change_map_jump_in(s32 duration) {
    g_battle_previous_game_state = g_battle_game_state;
    g_battle_game_state = BATTLE_GAME_STATE_MAP_JUMPING_IN_2;
    g_battle_state_screen_fade_step = duration != 0 ? 0x100 / duration : 0x100;
}
