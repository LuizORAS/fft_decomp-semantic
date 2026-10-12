#include "fft/battle_action.h"
#include "fft/unit.h"
#include "psx/types.h"

/* Formula 0x57: +1 level (a miss at level 99) and the ability's status on the
 * caster. A full HP and MP heal is stored first, but its action type is then
 * overwritten by the level change's. */
void battle_formula_raise_level_by_one_add_status_on_caster(void) {
    battle_action_data_t* action;
    battle_stats_t* saved_unit;
    battle_action_data_t* saved_action;
    battle_stats_t* attacker;
    battle_action_data_t* attacker_action;
    s32 one;

    battle_formula_apply_full_hp_mp_heal();
    battle_formula_apply_undead_reversal();
    one = 1;
    if (g_battle_action_target->level >= UNIT_LEVEL_MAX) {
        g_battle_action_target_data->attack_type = 0;
        battle_formula_force_attack_miss();
        return;
    }
    action = g_battle_action_target_data;
    action->attack_type = one;
    saved_unit = g_battle_action_target;
    saved_action = g_battle_action_target_data;
    attacker = g_battle_action_attacker;
    attacker_action = g_battle_action_attacker_data;
    /* The target writes the combined special-effect field as one 16-bit value. */
    action->special_effect = BATTLE_ACTION_SPECIAL_EFFECT_LEVEL_UP;
    g_battle_action_target = attacker;
    g_battle_action_target_data = attacker_action;
    attacker_action->hit = one;
    battle_formula_apply_status_to_action();
    g_battle_action_target = saved_unit;
    g_battle_action_target_data = saved_action;
}
