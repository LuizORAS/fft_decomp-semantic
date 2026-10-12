#include "fft/battle_ai.h"

/*
 * Reset an action's ranking fields and selection defaults.
 *
 * The remaining payload is retained; this is not a whole-record clear.
 */
void ai_reset_action_ranking_fields(ai_action_data_t* action) {
    /* Signed view preserves addiu -32767; direct u16 assignment emits ori. */
    action->rank.priority = -0x7fff;
    action->rank.decided = 0;
    action->rank.base_hit_percent = 0;
    action->wait_facing_hint = AI_DIRECTION_CHOOSE_AT_WAIT;
    action->reflected_action = 0;
}
