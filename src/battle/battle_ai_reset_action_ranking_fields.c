#include "fft/battle.h"

/* Reset an action's ranking fields: the lowest score, no hit chance, plan not chosen, facing chosen
 * at Wait, not reflected. The rest of the record is kept. */
void battle_ai_reset_action_ranking_fields(battle_ai_action_data_t* action) {
    action->priority = -0x7fff;
    action->plan_chosen = 0;
    action->base_hit_percent = 0;
    action->wait_facing_hint = BATTLE_AI_DIRECTION_CHOOSE_AT_WAIT;
    action->reflected_action = 0;
}
