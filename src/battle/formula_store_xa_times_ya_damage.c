#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Sets the target's result to HP damage XA * YA. */
void formula_store_xa_times_ya_damage(void) {
    s16 damage;
    action_data_t* action;

    damage = g_action_current_ability.xa * g_action_current_ability.ya;
    action = g_action_target_data;
    action->attack_type = ACTION_TYPE_HP_DAMAGE;
    action->hp_damage = damage;
}
