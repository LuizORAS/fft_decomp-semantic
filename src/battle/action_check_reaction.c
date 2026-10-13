#include "fft/battle_action.h"
#include "psx/types.h"

/* Whether a reaction is already in play: 1 while a weapon's proc ability is
 * being resolved, 2 when the unit's result already carries a reaction, 0
 * otherwise. */
s32 action_check_reaction(unit_stats_t* unit) {
    s32 result;
    /* Both globals are loaded unsigned (lhu) here. */
    if (g_action_current_ability.reaction_id != 0) {
        if (g_action_current_ability.weapon_proc_pending == 0)
            return 1;
    }
    /* The target loads the reaction id signed (lh). */
    result = unit->action.reaction_id;
    if (result != 0)
        result = 2;
    return result;
}
