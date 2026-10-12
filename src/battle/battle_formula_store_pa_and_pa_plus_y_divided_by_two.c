#include "fft/battle_action.h"

/* XA = PA, YA = (PA + Y) / 2. */
void battle_formula_store_pa_and_pa_plus_y_divided_by_two(void) {
    g_battle_action_current_ability.xa = g_battle_action_attacker->attributes[UNIT_ATTRIBUTE_PHYSICAL_ATTACK];
    g_battle_action_current_ability.ya = (g_battle_action_attacker->attributes[UNIT_ATTRIBUTE_PHYSICAL_ATTACK]
                                             + g_battle_action_current_ability.range_data.y)
        >> 1;
}
