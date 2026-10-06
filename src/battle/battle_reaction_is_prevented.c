#include "fft/battle.h"

/* Nonzero when the unit cannot react: during a reaction or a simulation (any context but the
 * primary action), or under a status of the PREVENT_REACTION set. */
s32 battle_reaction_is_prevented(const battle_stats_t* unit) {
    if (g_battle_action_context != BATTLE_ACTION_CONTEXT_PRIMARY) {
        return 1;
    }
    return main_unit_has_status_in_set(unit, MAIN_STATUS_CHECK_SET_PREVENT_REACTION);
}
