#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Target statuses on physical XA: Protect * 2/3; Sleep or Charging * 3/2;
 * Chicken or Frog * 3/2. */
void formula_apply_target_xa_affecting_statuses(void) {
    battle_stats_t* target;

    if (g_action_target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_PROTECT)]
        & STATUS_BYTE_MASK(STATUS_ID_PROTECT)) {
        g_action_current_ability.xa = g_action_current_ability.xa * 2 / 3;
    }

    target = g_action_target;
    if ((target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_SLEEP)] & STATUS_BYTE_MASK(STATUS_ID_SLEEP))
        || (target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CHARGING)]
            & STATUS_BYTE_MASK(STATUS_ID_CHARGING))) {
        g_action_current_ability.xa = g_action_current_ability.xa * 3 / 2;
    }

    if (g_action_target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CHICKEN)]
        & (STATUS_BYTE_MASK(STATUS_ID_CHICKEN) | STATUS_BYTE_MASK(STATUS_ID_FROG))) {
        g_action_current_ability.xa = g_action_current_ability.xa * 3 / 2;
    }
}
