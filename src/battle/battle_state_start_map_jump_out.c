#include "fft/battle.h"

/* Fade the screen out, 0x100 / duration a frame (one frame for a duration of 0), load map_id and
 * fade back in (MAP_JUMPING_OUT, MAP_INITIALIZE, MAP_JUMPING_IN), then return to the current
 * state. */
void battle_state_start_map_jump_out(s32 map_id, s32 duration) {
    g_battle_map_id = map_id;
    g_battle_previous_game_state = g_battle_game_state;
    g_battle_game_state = BATTLE_GAME_STATE_MAP_JUMPING_OUT;
    g_battle_state_screen_fade_step = duration != 0 ? 0x100 / duration : 0x100;
}
