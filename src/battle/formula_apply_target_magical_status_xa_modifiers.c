#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Target statuses on magical XA: Shell * 2/3; Chicken or Frog * 3/2. */
void formula_apply_target_magical_status_xa_modifiers(void) {
    if (g_action_target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_SHELL)] & STATUS_BYTE_MASK(STATUS_ID_SHELL)) {
        g_action_current_ability.xa = g_action_current_ability.xa * 2 / 3;
    }
    if (g_action_target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CHICKEN)]
        & (STATUS_BYTE_MASK(STATUS_ID_CHICKEN) | STATUS_BYTE_MASK(STATUS_ID_FROG))) {
        g_action_current_ability.xa = g_action_current_ability.xa * 3 / 2;
    }
}
