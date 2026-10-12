#include "fft/battle_action.h"
#include "psx/types.h"

/* Returns the first nonzero result of the three checks, or the action
 * restriction check's zero. */
s32 action_is_reaction_blocked(battle_stats_t* unit) {
    s32 result;

    if ((result = action_can_unit_react(unit)) == 0 && (result = action_check_reaction(unit)) == 0) {
        result = battle_unit_get_action_restriction(unit);
    }
    return result;
}
