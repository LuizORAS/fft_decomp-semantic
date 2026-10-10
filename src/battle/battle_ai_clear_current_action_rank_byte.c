#include "fft/battle.h"

/* Mark the candidate action as not decided on: only the selected action ever
 * holds 0xff there (battle_ai_build_command). */
void battle_ai_clear_current_action_rank_byte(void) {
    g_battle_ai_data_base.current_action.rank.rank_byte = 0;
}
