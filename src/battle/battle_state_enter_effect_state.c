#include "fft/battle.h"
#include "psx/types.h"

/* Switch to 30 fps and the EFFECT state, and mark the AT list active. */
void battle_state_enter_effect_state(void) {
    g_battle_state_vsync_interval = 2;
    g_battle_game_state = BATTLE_GAME_STATE_EFFECT;
    battle_action_set_at_list_active();
}
