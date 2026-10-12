#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* XA = MA, YA = the ability's X. */
void formula_store_ma_and_x(void) {
    s32 ma = g_action_attacker->attributes[UNIT_ATTRIBUTE_MAGIC_ATTACK];
    s32 x = g_action_current_ability.range_data.x;

    g_action_current_ability.ya = x;
    g_action_current_ability.xa = ma;
}
