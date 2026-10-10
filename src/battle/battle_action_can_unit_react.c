#include "fft/battle.h"

/* Nonzero when the unit cannot react: it has a status of the prevent-reaction
 * set, or the current action is itself a reaction or a simulation (anything but
 * the primary context), which never provokes one. */
s32 battle_action_can_unit_react(const battle_stats_t* unit) {
    if (g_battle_action_context != BATTLE_ACTION_CONTEXT_PRIMARY) {
        return 1;
    }
    return main_unit_has_status_in_set(unit, MAIN_STATUS_CHECK_SET_PREVENT_REACTION);
}
