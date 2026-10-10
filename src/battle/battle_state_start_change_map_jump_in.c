#include "fft/battle.h"

/* Start the fade-in that ends an event's map change (MapJumpingIn2), saving the
 * current game state to return to. The screen lightens by 0x100 / duration per
 * frame; a duration of 0 takes one frame. */
void battle_state_start_change_map_jump_in(s32 duration) {
    s32 previous_state;

    previous_state = g_battle_game_state;
    g_battle_game_state = BATTLE_GAME_STATE_MAP_JUMPING_IN_2;
    g_battle_previous_game_state = previous_state;
    g_battle_state_screen_fade_step = duration != 0 ? 0x100 / duration : 0x100;
}
