#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* The target's Abandon, when it can act (battle_unit_get_action_block): halves the base hit, which
 * doubles its evades, and records Abandon as the reaction. */
void battle_formula_calculate_abandon(void) {
    u8* base_hit;
    if ((battle_unit_get_action_block(g_battle_action_target) == BATTLE_UNIT_ACTION_BLOCK_NONE)
        && (g_battle_action_target->ability_flags.reaction[3] & BATTLE_REACTION_SET_4_ABANDON)) {
        base_hit = &g_battle_action_current_ability.base_hit;
        *base_hit = *base_hit >> 1;
        g_battle_action_target_data->reaction_id = ABILITY_ID_REACTION_ABANDON;
    }
}
