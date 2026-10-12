#include "fft/battle.h"

/* Enter the Effect game state at two vertical blanks per frame. No code calls it
 * or battle_state_enter_effect_playback, the only other entry to that state. */
void battle_state_halve_animation_speed_for_effects(void) {
    g_main_system_animation_speed = 2;
    g_battle_game_state = BATTLE_GAME_STATE_EFFECT;
    action_set_at_list_active();
}
