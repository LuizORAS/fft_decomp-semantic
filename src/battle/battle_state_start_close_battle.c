#include "fft/battle.h"

/* Leave the battle for the world map: enter CloseBattle at two vertical blanks
 * per frame with a fade step of 0x100 / duration (0x100 for a duration of 0). */
void battle_state_start_close_battle(s32 duration) {
    g_main_system_animation_speed = 2;
    g_battle_game_state = BATTLE_GAME_STATE_CLOSE_BATTLE;
    g_battle_state_screen_fade_step = duration != 0 ? 0x100 / duration : 0x100;
    g_main_system_game_flow_state = MAIN_GAME_FLOW_WORLD;
}
