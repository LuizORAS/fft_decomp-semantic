#include "fft/battle.h"
#include "psx/types.h"

/* HP damage of a random 1 to Y times XA ((Y / 2 + 1) * XA in an estimate). While executing, the
 * hit knocks the target back (battle_formula_calculate_knockback) when a random 0 to the target's
 * Brave - 1 falls below a random 0 to the attacker's Brave - 1. */
void battle_formula_apply_damage_and_knockback(void) {
    battle_action_data_t* action;
    s32 damage;
    s32 roll;
    s32 state;

    damage
        = (((battle_formula_get_random_0_7fff() * g_current_ability.range_data.y) / 32768) + 1) * g_current_ability.xa;
    action = g_battle_action_target_data;
    action->attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE;
    state = g_battle_action_state;
    action->hp_damage = (s16)damage;
    if (state != BATTLE_ACTION_STATE_EXECUTE) {
        return;
    }
    roll = (battle_formula_get_random_0_7fff() * g_battle_action_attacker->brave) / 32768;
    if (main_util_roll_pass_fail(g_battle_action_target->brave, roll) == 0) {
        battle_formula_calculate_knockback();
    }
}
