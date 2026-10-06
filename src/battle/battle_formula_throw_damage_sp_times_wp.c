#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x63, the Throw command (battle_action_run_pre_formula_setup selects it): Catch may take
 * the thrown weapon first (battle_formula_apply_catch); then the physical evade check, XA = Speed and
 * YA = the thrown item's power, the physical status modifiers, XA * YA, the target's affinities for
 * the item's element and the absorption. No critical hit or status. */
void battle_formula_throw_damage_sp_times_wp(void) {
    battle_formula_apply_catch();
    if (g_battle_action_target_data->hit == 0) {
        return;
    }
    if (battle_formula_calculate_physical_evade() != 0) {
        return;
    }
    g_current_ability.xa = g_battle_action_attacker->attributes[UNIT_ATTRIBUTE_SPEED];
    g_current_ability.ya = g_current_ability.weapon_data.power;
    battle_formula_apply_physical_status_xa_modifiers();
    battle_formula_store_xa_times_ya_damage();
    battle_formula_apply_weapon_element();
    if (g_battle_action_target_data->hit == 0) {
        return;
    }
    battle_formula_apply_elemental_absorption();
}
