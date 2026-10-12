#include "fft/battle_action.h"

/* Strengthen step: XA * 5/4 when the attacker strengthens one of the ability's
 * elements. */
void formula_apply_elemental_strengthen(void) {
    if (g_action_attacker->elemental_affinity[ELEMENTAL_AFFINITY_STRENGTHEN]
        & g_action_current_ability.range_data.element) {
        g_action_current_ability.xa = g_action_current_ability.xa * 5 / 4;
    }
}
