#include "fft/battle.h"
#include "psx/types.h"

/* Apply Abandon by halving the target's current evasion contribution. */
void battle_formula_calculate_abandon(void) {
    u8* evade_counter;
    if ((battle_unit_get_action_restriction(g_battle_action_target) == 0)
        && (g_battle_action_target->reaction_abilities[3] & BATTLE_REACTION_SET_4_ABANDON)) {
        evade_counter = &g_battle_action_current_ability.base_hit;
        *evade_counter = *evade_counter >> 1;
        g_battle_action_target_data->reaction_id = ABILITY_ID_REACTION_ABANDON;
    }
}
