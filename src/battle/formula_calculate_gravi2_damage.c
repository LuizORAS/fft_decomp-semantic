#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* HP damage equal to the target's current HP - 1 (0 when it has none). */
void formula_calculate_gravi2_damage(void) {
    unit_stats_t* target;

    target = g_action_target;
    if (target->hp != 0) {
        g_action_target_data->hp_damage = g_action_target->hp - 1;
    } else {
        g_action_target_data->hp_damage = 0;
    }
    g_action_target_data->attack_type = ACTION_TYPE_HP_DAMAGE;
}
