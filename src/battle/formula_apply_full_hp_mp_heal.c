#include "fft/battle_action.h"

/* Restore the target to full HP and MP (100% HP/MP healing). */
void formula_apply_full_hp_mp_heal(void) {
    battle_stats_t* unit = g_action_target;
    action_data_t* action = g_action_target_data;

    action->hp_damage = unit->max_hp;
    action->mp_healing = unit->max_mp;
    action->attack_type = ACTION_TYPE_HP_DAMAGE | ACTION_TYPE_MP_HEALING;
}
