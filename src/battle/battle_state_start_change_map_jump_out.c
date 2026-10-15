#include "fft/battle.h"

/* Same as battle_state_start_map_jump_out (MAP_JUMPING_OUT_2), but the current state returns
 * with the screen still black, until battle_state_start_change_map_jump_in. */
void battle_state_start_change_map_jump_out(s32 map_id, s32 duration) {
    s32 previous_state;

    previous_state = g_battle_game_state;
    g_battle_map_id = map_id;
    g_battle_game_state = BATTLE_GAME_STATE_MAP_JUMPING_OUT_2;
    g_battle_previous_game_state = previous_state;
    g_battle_state_screen_fade_step = duration != 0 ? 0x100 / duration : 0x100;
}
