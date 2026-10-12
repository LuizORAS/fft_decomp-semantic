#include "fft/battle_action.h"
#include "psx/types.h"

/* XA = MA, YA = the ability's Y. */
void battle_formula_store_ma_and_y(void) {
    s32 ma = g_action_attacker->attributes[UNIT_ATTRIBUTE_MAGIC_ATTACK];
    s32 y = g_action_current_ability.range_data.y;

    g_action_current_ability.ya = y;
    g_action_current_ability.xa = ma;
}
