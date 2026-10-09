#include "fft/battle.h"

void battle_formula_apply_target_xa_affecting_statuses(void) {
    battle_stats_t* target;

    if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_PROTECT)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PROTECT)) {
        s16* xa = (s16*)&g_battle_action_current_ability.xa;

        *xa = *xa * 2 / 3;
    }

    target = g_battle_action_target;
    if ((target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_SLEEP)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SLEEP))
        || (target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARGING)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARGING))) {
        s16* xa = (s16*)&g_battle_action_current_ability.xa;

        *xa = *xa * 3 / 2;
    }

    if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHICKEN)]
        & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHICKEN) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG))) {
        s16* xa = (s16*)&g_battle_action_current_ability.xa;

        *xa = *xa * 3 / 2;
    }
}
