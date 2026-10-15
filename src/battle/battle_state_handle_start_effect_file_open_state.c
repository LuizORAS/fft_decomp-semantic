#include "fft/battle.h"
#include "psx/types.h"

/* START_EFFECT_FILE_OPEN: wait until battle_effect_update_file_load reports the effect file ready,
 * then pick the attack animation and resolve the targets
 * (battle_action_apply_strike_results). */
void battle_state_handle_start_effect_file_open_state(void) {
    if (battle_effect_update_file_load() != 0) {
        g_battle_state_animation_continue_check = 1;
    } else {
        g_battle_state_animation_continue_check = 0;
    }
    if (g_battle_state_animation_continue_check == 0) {
        battle_action_apply_strike_results();
    }
    battle_state_update_units();
}
