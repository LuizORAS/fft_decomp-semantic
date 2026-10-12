#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* XA = MA, YA = (PA + Y) / 2. */
void formula_store_ma_and_pa_plus_y_divided_by_two(void) {
    g_action_current_ability.xa = g_action_attacker->attributes[UNIT_ATTRIBUTE_MAGIC_ATTACK];
    g_action_current_ability.ya
        = (g_action_attacker->attributes[UNIT_ATTRIBUTE_PHYSICAL_ATTACK] + g_action_current_ability.range_data.y) >> 1;
}
