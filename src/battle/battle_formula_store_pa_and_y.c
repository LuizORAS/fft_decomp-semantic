#include "fft/battle.h"
#include "psx/types.h"

/* Set XA = PA and YA = Y, Y from the ability's data. */
void battle_formula_store_pa_and_y(void) {
    s32 pa = g_battle_action_attacker->attributes[UNIT_ATTRIBUTE_PHYSICAL_ATTACK];
    s32 y = g_current_ability.range_data.y;

    g_current_ability.ya = y;
    g_current_ability.xa = pa;
}
