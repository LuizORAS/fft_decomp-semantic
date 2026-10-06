#include "fft/battle.h"
#include "psx/types.h"

/* Nonzero when the unit cannot react now: the first nonzero of battle_reaction_is_prevented, a
 * reaction already pending (battle_reaction_get_pending) and battle_unit_get_action_block (the unit
 * cannot act). Else 0. */
s32 battle_reaction_is_blocked(battle_stats_t* unit) {
    s32 result;

    if ((result = battle_reaction_is_prevented(unit)) == 0 && (result = battle_reaction_get_pending(unit)) == 0) {
        result = battle_unit_get_action_block(unit);
    }
    return result;
}
