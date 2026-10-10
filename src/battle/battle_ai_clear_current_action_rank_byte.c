#include "fft/battle.h"

/* Mark the candidate action as not decided on: only the selected action ever
 * holds BATTLE_AI_ACTION_DECIDED (battle_ai_build_command). */
void battle_ai_clear_current_action_rank_byte(void) {
    g_battle_ai_data_base.current_action.rank.decided = 0;
}
