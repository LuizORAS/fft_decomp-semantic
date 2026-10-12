#include "fft/battle.h"

/* Start an event's change to map_id (MapJumpingOut2), saving the current game
 * state: the screen darkens by 0x100 / duration per frame (a duration of 0 takes
 * one frame), the map loads, and the saved state resumes behind a black screen
 * until battle_state_start_change_map_jump_in fades it in. */
void battle_state_start_change_map_jump_out(s32 map_id, s32 duration) {
    s32 previous_state;

    previous_state = g_battle_game_state;
    g_map_id = map_id;
    g_battle_game_state = BATTLE_GAME_STATE_MAP_JUMPING_OUT_2;
    g_battle_previous_game_state = previous_state;
    g_battle_state_screen_fade_step = duration != 0 ? 0x100 / duration : 0x100;
}
