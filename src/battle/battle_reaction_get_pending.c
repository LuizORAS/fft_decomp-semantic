#include "fft/battle.h"
#include "psx/types.h"

/* Which reaction is already pending: 1 when the strike itself carries a reaction id (and no weapon
 * spell waits), 2 when the unit's result already holds one, else 0. */
s32 battle_reaction_get_pending(battle_stats_t* unit) {
    s32 result;
    /* Both globals are loaded unsigned (lhu) here. */
    if (g_current_ability.reaction_id != 0) {
        if (g_current_ability.weapon_spell_pending == 0)
            return 1;
    }
    result = unit->action.reaction_id;
    if (result != 0)
        result = 2;
    return result;
}
