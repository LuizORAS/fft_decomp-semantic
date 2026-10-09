#include "fft/battle.h"
#include "psx/types.h"

s32 battle_action_check_reaction(battle_stats_t* unit) {
    s32 result;
    /* Both globals are loaded unsigned (lhu) here. */
    if (g_battle_action_current_ability.reaction_id != 0) {
        if (g_battle_action_current_ability.weapon_proc_pending == 0)
            return 1;
    }
    /* The target loads the reaction id signed (lh). */
    result = unit->action.reaction_id;
    if (result != 0)
        result = 2;
    return result;
}
