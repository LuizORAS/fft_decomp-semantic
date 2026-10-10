#include "fft/battle.h"

/* Clear the current action's plan_chosen byte (g_battle_ai_current_action_plan_chosen) before
 * battle_ai_simulate_action_and_score scores it. */
void battle_ai_clear_current_action_plan_chosen(void) {
    g_battle_ai_current_action_plan_chosen = 0;
}
