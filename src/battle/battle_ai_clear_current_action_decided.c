#include "fft/battle_ai.h"

/* Clear the current action's decided byte (current_action.rank.decided) before
 * battle_ai_simulate_action_and_score scores it: only the selected action ever holds
 * BATTLE_AI_ACTION_DECIDED (battle_ai_build_command). */
void battle_ai_clear_current_action_decided(void) {
    g_battle_ai_data_base.current_action.rank.decided = 0;
}
