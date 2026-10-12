#include "fft/battle_action.h"
#include "fft/unit.h"

/* Formula 0x63: thrown weapon damage (SP * WP) with the weapon's element;
 * Catch, then physical evade. */
void formula_damage_sp_times_wp(void) {
    formula_apply_catch();
    if (g_action_target_data->hit == 0) {
        return;
    }
    if (formula_calculate_physical_evade() != 0) {
        return;
    }
    g_action_current_ability.xa = g_action_attacker->attributes[UNIT_ATTRIBUTE_SPEED];
    g_action_current_ability.ya = g_action_current_ability.weapon_data.power;
    formula_apply_physical_status_support_compatibility();
    formula_store_xa_times_ya_damage();
    formula_modify_elemental_damage();
    if (g_action_target_data->hit == 0) {
        return;
    }
    formula_apply_elemental_absorption();
}
