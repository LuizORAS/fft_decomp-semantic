#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* XA = Speed, YA = the ability's X. */
void formula_store_speed_and_x(void) {
    s32 speed = g_action_attacker->attributes[UNIT_ATTRIBUTE_SPEED];
    s32 x = g_action_current_ability.range_data.x;

    g_action_current_ability.ya = x;
    g_action_current_ability.xa = speed;
}
