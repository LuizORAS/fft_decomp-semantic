#include "fft/battle.h"

/* The attacker's Strengthen for the ability's element: XA * 5 / 4. */
void battle_formula_apply_ability_element_strengthen(void) {
    if (g_battle_action_attacker->elemental_affinity[ELEMENTAL_AFFINITY_STRENGTHEN]
        & g_current_ability.range_data.element) {
        g_current_ability.xa = (s16)g_current_ability.xa * 5 / 4;
    }
}
