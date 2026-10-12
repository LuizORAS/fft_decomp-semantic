#include "fft/battle_action.h"

/* XA = MA, YA = (MA + Y) / 2. */
void battle_formula_store_ma_and_ma_plus_y_divided_by_two(void) {
    g_battle_action_current_ability.xa = g_battle_action_attacker->attributes[UNIT_ATTRIBUTE_MAGIC_ATTACK];
    g_battle_action_current_ability.ya = (g_battle_action_attacker->attributes[UNIT_ATTRIBUTE_MAGIC_ATTACK]
                                             + g_battle_action_current_ability.range_data.y)
        >> 1;
}
