#include "fft/battle.h"

/* Switch to 30 fps and the CLOSE_BATTLE state, fading 0x100 / duration a frame (one frame for a
 * duration of 0), and return to the world map (game flow 1). Event instructions call it. */
void battle_state_start_close_battle(s32 duration) {
    g_main_system_vsync_interval = 2;
    g_battle_game_state = BATTLE_GAME_STATE_CLOSE_BATTLE;
    g_battle_state_screen_fade_step = duration != 0 ? 0x100 / duration : 0x100;
    g_main_system_game_flow_state = MAIN_GAME_FLOW_WORLD;
}
