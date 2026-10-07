#include "fft/battle.h"
#include "psx/types.h"

/* HP damage of the target's current HP - 1 (0 when it has none), so the hit leaves 1 HP. Gravi 2
 * (formula 0x17) and formula 0x3e use it. */
void battle_formula_calculate_damage_leaving_one_hp(void) {
    battle_stats_t* target;

    target = g_battle_action_target;
    if (target->hp != 0) {
        g_battle_action_target_data->hp_damage = (s16)(g_battle_action_target->hp - 1);
    } else {
        g_battle_action_target_data->hp_damage = 0;
    }
    g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE;
}
