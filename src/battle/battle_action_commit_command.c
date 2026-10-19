#include "fft/battle_action.h"
#include "fft/battle_status.h"
#include "psx/types.h"

/* Commit a command to its unit (battle_action_prepare_attack, phase 1): the resolved action goes
 * to the unit's battle record (from action_command) with its charge time, and the unit is
 * marked as having acted. When the ability acts at once in a primary action, the unit's
 * Charging, Jumping, Defending and Performing statuses are cleared
 * (battle_status_clear_action_state). Returns the battle_attack_preparation_e result.
 * battle_action_execute_ability runs a command this way; the AT list preview, AI scoring and
 * reactions call it around saved state. */
s32 battle_action_commit_command(action_command_t* command) {
    battle_stats_t* unit;
    s32 result;

    unit = &g_battle_unit_stats[command->actor_id];
    result = battle_action_prepare_attack(command, &unit->action_command, 1);
    if (result == BATTLE_ATTACK_PREPARATION_IMMEDIATE && g_battle_action_context == BATTLE_ACTION_CONTEXT_PRIMARY) {
        battle_status_clear_action_state(unit);
    }
    return result;
}
