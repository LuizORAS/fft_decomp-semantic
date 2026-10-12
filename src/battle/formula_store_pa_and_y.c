#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* XA = PA, YA = the ability's Y. */
void formula_store_pa_and_y(void) {
    s32 pa = g_action_attacker->attributes[UNIT_ATTRIBUTE_PHYSICAL_ATTACK];
    s32 y = g_action_current_ability.range_data.y;

    g_action_current_ability.ya = y;
    g_action_current_ability.xa = pa;
}
