#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* The attacker's Strengthen for the weapon's element: XA * 5 / 4. */
void battle_formula_apply_weapon_element_strengthen(void) {
    if (g_battle_action_attacker->elemental_affinity[ELEMENTAL_AFFINITY_STRENGTHEN]
        & g_battle_action_current_ability.weapon_data.element) {
        g_battle_action_current_ability.xa = g_battle_action_current_ability.xa * 5 / 4;
    }
}
