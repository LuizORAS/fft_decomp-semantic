#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "fft/unit.h"
#include "psx/types.h"

/* Formula 0x57: +1 level (a miss at level 99) and the ability's status on the
 * caster. A full HP and MP heal is stored first, but its action type is then
 * overwritten by the level change's. */
void formula_raise_level_by_one_add_status_on_caster(void) {
    action_data_t* action;
    battle_stats_t* saved_unit;
    action_data_t* saved_action;
    battle_stats_t* attacker;
    action_data_t* attacker_action;

    formula_apply_full_hp_mp_heal();
    formula_apply_undead_reversal();
    if (g_action_target->level >= UNIT_LEVEL_MAX) {
        g_action_target_data->attack_type = 0;
        formula_force_attack_miss();
        return;
    }
    action = g_action_target_data;
    action->attack_type = ACTION_TYPE_PSEUDO_STATUS;
    saved_unit = g_action_target;
    saved_action = g_action_target_data;
    attacker = g_action_attacker;
    attacker_action = g_action_attacker_data;
    /* The target writes the combined special-effect field as one 16-bit value. */
    action->special_effect = ACTION_SPECIAL_EFFECT_LEVEL_UP;
    g_action_target = attacker;
    g_action_target_data = attacker_action;
    attacker_action->hit = 1;
    formula_apply_status_to_action();
    g_action_target = saved_unit;
    g_action_target_data = saved_action;
}
