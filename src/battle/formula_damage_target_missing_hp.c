#include "fft/battle_action.h"

/* Formula 0x45: damage equal to the target's missing HP (MaxHP - CurHP). */
void formula_damage_target_missing_hp(void) {
    battle_stats_t* unit = g_action_target;
    action_data_t* action = g_action_target_data;

    action->hp_damage = unit->max_hp - unit->hp;
    action->attack_type = ACTION_TYPE_HP_DAMAGE;
}
