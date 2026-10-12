#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Damage XA * rnd(1..Y). When executing, the target is knocked back if a roll
 * of 0..(its Brave - 1) comes out under a roll of 0..(attacker Brave - 1). */
void formula_apply_damage_and_knockback(void) {
    action_data_t* action;
    s32 damage;
    s32 roll;
    s32 state;

    damage = (((formula_get_random_0_7fff() * g_action_current_ability.range_data.y) / 32768) + 1)
        * (u16)g_action_current_ability.xa;
    action = g_action_target_data;
    action->attack_type = ACTION_TYPE_HP_DAMAGE;
    state = g_action_state;
    action->hp_damage = damage;
    if (state != ACTION_STATE_EXECUTE) {
        return;
    }
    roll = (formula_get_random_0_7fff() * g_action_attacker->brave) / 32768;
    if (main_util_roll_pass_fail(g_action_target->brave, roll) == 0) {
        formula_calculate_knockback();
    }
}
