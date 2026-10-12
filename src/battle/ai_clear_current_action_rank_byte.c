#include "fft/battle_ai.h"

/* Mark the candidate action as not decided on: only the selected action ever
 * holds AI_ACTION_DECIDED (ai_build_command). */
void ai_clear_current_action_rank_byte(void) {
    g_ai_data_base.current_action.rank.decided = 0;
}
