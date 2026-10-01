#include "fft/battle.h"
#include "psx/types.h"

/* Returns the first nonzero result of the three checks, or the evade check's
 * zero. */
s32 battle_action_can_unit_react_1(battle_stats_t* unit) {
    s32 result;

    if ((result = battle_action_can_unit_react(unit)) == 0 && (result = battle_action_check_reaction(unit)) == 0) {
        result = battle_formula_can_unit_evade(unit);
    }
    return result;
}
