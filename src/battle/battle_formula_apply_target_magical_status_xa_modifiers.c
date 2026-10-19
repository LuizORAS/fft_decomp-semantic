#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* The target's statuses on a magical XA: Shell * 2 / 3; Chicken or Frog * 3 / 2. */
void battle_formula_apply_target_magical_status_xa_modifiers(void) {
    if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_SHELL)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SHELL)) {
        g_battle_action_current_ability.xa = g_battle_action_current_ability.xa * 2 / 3;
    }
    if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHICKEN)]
        & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHICKEN) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG))) {
        g_battle_action_current_ability.xa = g_battle_action_current_ability.xa * 3 / 2;
    }
}
