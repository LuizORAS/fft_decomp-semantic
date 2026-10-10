#include "fft/battle.h"
#include "psx/types.h"

/* Enter the Effect game state at two vertical blanks per frame. No code calls it;
 * battle_state_enter_effect_playback is the entry in use. */
void battle_state_halve_animation_speed_for_effects(void) {
    g_main_system_animation_speed = 2;
    g_battle_game_state = BATTLE_GAME_STATE_EFFECT;
    battle_action_set_at_list_active();
}
