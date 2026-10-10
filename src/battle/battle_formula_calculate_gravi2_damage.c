#include "fft/battle.h"
#include "psx/types.h"

/* HP damage equal to the target's current HP - 1 (0 when it has none). */
void battle_formula_calculate_gravi2_damage(void) {
    battle_stats_t* target;

    target = g_battle_action_target;
    if (target->hp != 0) {
        g_battle_action_target_data->hp_damage = g_battle_action_target->hp - 1;
    } else {
        g_battle_action_target_data->hp_damage = 0;
    }
    g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE;
}
