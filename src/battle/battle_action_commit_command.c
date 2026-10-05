#include "fft/battle.h"
#include "psx/types.h"

/* Commit a command to its unit (battle_action_prepare_attack, phase 1): the resolved action goes to
 * the unit's battle record (from action_actor_id) with its charge time, and the unit is marked as
 * having acted. When the ability acts at once in a primary action, the unit's Charging, Jumping,
 * Defending and Performing statuses are cleared (battle_status_enable_disable_acting).
 * battle_action_execute_ability runs a command this way; the AT list preview, AI scoring and
 * reactions call it around saved state. */
s32 battle_action_commit_command(u8* command) {
    battle_stats_t* unit;
    s32 result;

    unit = &g_battle_unit_stats[*command];
    result = battle_action_prepare_attack(
        (battle_ai_command_action_t*)command, (battle_ai_command_action_t*)&unit->action_actor_id, 1);
    if (result == 1 && g_battle_action_context == BATTLE_ACTION_CONTEXT_PRIMARY) {
        battle_status_enable_disable_acting(unit);
    }
    return result;
}
