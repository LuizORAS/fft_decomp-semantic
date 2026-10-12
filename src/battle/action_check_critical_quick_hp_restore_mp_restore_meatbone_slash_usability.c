#include "fft/battle_action.h"
#include "psx/types.h"

/* Record reaction_id for a target that is in Critical, is taking HP damage and
 * passes its reaction roll. */
void action_check_critical_quick_hp_restore_mp_restore_meatbone_slash_usability(s16 reaction_id) {
    battle_stats_t* unit;

    unit = g_action_target;
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRITICAL)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRITICAL))
        && (g_action_target_data->attack_type & ACTION_TYPE_HP_DAMAGE) && action_calculate_chance_to_react(unit) == 0) {
        g_action_target_data->reaction_id = reaction_id;
    }
}
