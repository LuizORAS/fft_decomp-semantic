#include "fft/battle.h"
#include "psx/types.h"

/* The target's Abandon, when it can act (battle_unit_get_action_block): halves the base hit, which
 * doubles its evades, and records Abandon as the reaction. */
void battle_formula_calculate_abandon(void) {
    u8* evade_counter;
    if ((battle_unit_get_action_block(g_battle_action_target) == 0)
        && (g_battle_action_target->reaction_abilities[3] & BATTLE_REACTION_SET_4_ABANDON)) {
        evade_counter = &g_current_ability.base_hit;
        *evade_counter = *evade_counter >> 1;
        g_battle_action_target_data->reaction_id = ABILITY_ID_REACTION_ABANDON;
    }
}
