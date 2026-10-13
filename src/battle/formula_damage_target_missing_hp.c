#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x45: damage equal to the target's missing HP (MaxHP - CurHP). */
void formula_damage_target_missing_hp(void) {
    unit_stats_t* unit = g_action_target;
    action_data_t* action = g_action_target_data;

    action->hp_damage = unit->max_hp - unit->hp;
    action->attack_type = ACTION_TYPE_HP_DAMAGE;
}
