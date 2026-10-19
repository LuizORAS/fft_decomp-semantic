#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Set XA = Speed and YA = X, X from the ability's data. */
void battle_formula_store_speed_and_x(void) {
    s32 speed = g_battle_action_attacker->attributes[UNIT_ATTRIBUTE_SPEED];
    s32 x = g_battle_action_current_ability.range_data.x;

    g_battle_action_current_ability.ya = x;
    g_battle_action_current_ability.xa = speed;
}
