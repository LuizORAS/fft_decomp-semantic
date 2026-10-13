#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x52: damage (CasMaxHP - CasCurHP), 100% status; caster in AoE takes its own current HP as damage. */
void formula_damage_caster_missing_hp_self_sacrifice(void) {
    if (g_action_target != g_action_attacker) {
        unit_stats_t* caster = g_action_attacker;
        action_data_t* action = g_action_target_data;

        action->hp_damage = caster->max_hp - caster->hp;
        action->attack_type = ACTION_TYPE_HP_DAMAGE;
    } else {
        action_data_t* action = g_action_target_data;

        action->hp_damage = g_action_target->hp;
        action->attack_type = ACTION_TYPE_HP_DAMAGE;
    }
    if (g_action_target != g_action_attacker) {
        formula_apply_status();
    }
}
