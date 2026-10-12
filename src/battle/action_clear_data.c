#include "fft/battle_action.h"

/* Reset the result records of the current target and the current attacker; the
 * attacker's starts as not hit. */
void action_clear_data(void) {
    action_clear_current_data(g_action_target_data);
    action_clear_current_data(g_action_attacker_data);
    g_action_attacker_data->hit = 0;
}
