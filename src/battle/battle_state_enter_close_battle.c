#include "fft/battle.h"
#include "psx/types.h"

/* Switch to 30 fps and the CLOSE_BATTLE state, fading by transition_step a frame (callers
 * pass 8: 32 frames), and set g_main_system_game_flow_state to close_flow_state. */
void battle_state_enter_close_battle(s32 transition_step, s32 close_flow_state) {
    g_battle_state_vsync_interval = 2;
    g_battle_game_state = BATTLE_GAME_STATE_CLOSE_BATTLE;
    g_battle_state_map_transition_step = transition_step;
    g_main_system_game_flow_state = close_flow_state;
}
