#include "fft/battle.h"
#include "psx/types.h"

/* HP Restore, MP Restore, Critical Quick and Meatbone Slash: when the target is Critical after HP
 * damage, a successful Brave roll queues reaction_id. */
void battle_reaction_try_while_critical(s16 reaction_id) {
    battle_stats_t* unit;

    unit = g_battle_action_target;
    if ((unit->status_sets.current[2] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRITICAL))
        && (g_battle_action_target_data->attack_type & BATTLE_ACTION_TYPE_HP_DAMAGE)
        && battle_reaction_fails_brave_roll(unit) == 0) {
        g_battle_action_target_data->reaction_id = reaction_id;
    }
}
