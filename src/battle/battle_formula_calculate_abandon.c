#include "fft/battle_action.h"
#include "psx/types.h"

/* Abandon: a target free to act halves the base hit, which doubles each of its
 * evades. */
void battle_formula_calculate_abandon(void) {
    u8* base_hit;
    if ((battle_unit_get_action_restriction(g_battle_action_target) == BATTLE_UNIT_ACTION_RESTRICTION_NONE)
        && (g_battle_action_target->reaction_abilities[3] & BATTLE_REACTION_SET_4_ABANDON)) {
        base_hit = &g_battle_action_current_ability.base_hit;
        *base_hit = *base_hit >> 1;
        g_battle_action_target_data->reaction_id = ABILITY_ID_REACTION_ABANDON;
    }
}
