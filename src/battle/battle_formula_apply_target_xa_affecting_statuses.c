#include "fft/battle_action.h"

/* Target statuses on physical XA: Protect * 2/3; Sleep or Charging * 3/2;
 * Chicken or Frog * 3/2. */
void battle_formula_apply_target_xa_affecting_statuses(void) {
    battle_stats_t* target;

    if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_PROTECT)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PROTECT)) {
        g_battle_action_current_ability.xa = g_battle_action_current_ability.xa * 2 / 3;
    }

    target = g_battle_action_target;
    if ((target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_SLEEP)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SLEEP))
        || (target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARGING)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARGING))) {
        g_battle_action_current_ability.xa = g_battle_action_current_ability.xa * 3 / 2;
    }

    if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHICKEN)]
        & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHICKEN) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG))) {
        g_battle_action_current_ability.xa = g_battle_action_current_ability.xa * 3 / 2;
    }
}
