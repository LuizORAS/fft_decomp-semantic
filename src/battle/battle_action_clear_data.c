#include "fft/battle_action.h"

/* Reset the result records of the current target and the current attacker; the
 * attacker's starts as not hit. */
void battle_action_clear_data(void) {
    battle_action_clear_current_data(g_battle_action_target_data);
    battle_action_clear_current_data(g_battle_action_attacker_data);
    g_battle_action_attacker_data->hit = 0;
}
