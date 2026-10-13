#include "fft/battle_action.h"
#include "fft/battle_status.h"
#include "psx/types.h"

/* Run action_prepare_attack phase 1 on a command, writing the prepared
 * action into the acting unit's own action fields.
 *
 * An action that resolves immediately also ends the unit's Charging, Jump,
 * Defending and Performing statuses, but only in the primary action context.
 * Returns the battle_attack_preparation_e result. */
s32 action_call_attack_preparation_at_preview(action_command_t* command) {
    unit_stats_t* unit;
    s32 result;

    unit = &g_battle_unit_stats[command->actor_id];
    result = action_prepare_attack(command, &unit->action_command, 1);
    if (result == BATTLE_ATTACK_PREPARATION_IMMEDIATE && g_action_context == ACTION_CONTEXT_PRIMARY) {
        status_enable_disable_acting(unit);
    }
    return result;
}
