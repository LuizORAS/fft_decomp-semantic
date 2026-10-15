#include "fft/battle.h"
#include "psx/types.h"

/* The Brave roll of a reaction: 0 when it triggers (a roll of 0-99 below the unit's Brave), 1 when it
 * does not. Outside an executing action (AI simulation, preview) it returns 0, so the reaction counts
 * as triggered there; the callers that change the result also require an executing action. */
s32 battle_reaction_fails_brave_roll(const battle_stats_t* unit) {
    if (g_battle_action_state != BATTLE_ACTION_STATE_EXECUTE) {
        return 0;
    }
    return main_util_roll_pass_fail(100, unit->brave);
}
