#include "fft/battle_action.h"

/* HP damage of Y% of the target's max HP, rounded up. */
void battle_formula_calculate_hp_percent_damage(void) {
    action_data_t* action;
    s32 damage;

    damage = (g_action_target->max_hp * g_action_current_ability.range_data.y + 99) / 100;
    action = g_action_target_data;
    action->attack_type |= ACTION_TYPE_HP_DAMAGE;
    action->hp_damage = damage;
}
