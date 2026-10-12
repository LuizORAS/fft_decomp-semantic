#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Attacker statuses on XA: Berserk * 3/2; a Frog's XA is 1. */
void formula_apply_attacker_berserk_frog(void) {
    if (g_action_attacker->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_BERSERK)]
        & STATUS_BYTE_MASK(STATUS_ID_BERSERK)) {
        g_action_current_ability.xa = g_action_current_ability.xa * 3 / 2;
    }
    if (g_action_attacker->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_FROG)] & STATUS_BYTE_MASK(STATUS_ID_FROG)) {
        g_action_current_ability.xa = 1;
    }
}
