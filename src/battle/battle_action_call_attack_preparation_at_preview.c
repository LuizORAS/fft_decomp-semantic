#include "fft/battle.h"
#include "psx/types.h"

/* Run battle_action_prepare_attack phase 1 on a command, writing the prepared
 * action into the acting unit's own action fields.
 *
 * An action that resolves immediately also ends the unit's Charging, Jump,
 * Defending and Performing statuses, but only in the primary action context.
 * Returns the battle_attack_preparation_e result. */
s32 battle_action_call_attack_preparation_at_preview(u8* command) {
    battle_stats_t* unit;
    s32 result;

    unit = &g_battle_unit_stats[*command];
    result = battle_action_prepare_attack(
        (battle_ai_command_action_t*)command, (battle_ai_command_action_t*)&unit->action_command, 1);
    if (result == BATTLE_ATTACK_PREPARATION_IMMEDIATE && g_battle_action_context == BATTLE_ACTION_CONTEXT_PRIMARY) {
        battle_status_enable_disable_acting(unit);
    }
    return result;
}
