#include "fft/battle_action.h"

/* Strengthen step for weapon attacks: XA * 5/4 when the attacker strengthens
 * one of the weapon's elements. */
void battle_formula_apply_weapon_element_strengthen(void) {
    if (g_action_attacker->elemental_affinity[ELEMENTAL_AFFINITY_STRENGTHEN]
        & g_action_current_ability.weapon_data.element) {
        g_action_current_ability.xa = g_action_current_ability.xa * 5 / 4;
    }
}
