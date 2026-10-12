#include "fft/battle.h"

/* Makes the attacker's own action result remove Transparent when its last
 * skillset is Jump and it carries Transparent as an inflicted status. Also points
 * g_battle_action_attacker_data at the attacker's result. */
void battle_status_remove_transparent_if_jump_used(battle_stats_t* unit) {
    battle_action_data_t* action = &g_battle_action_attacker_action_data;
    g_battle_action_attacker_data = action;
    /* Inflicted status byte 2 bit 0x10 is Transparent. */
    if (unit->action_command.skillset_id == SKILLSET_ID_JUMP
        && (unit->inflicted_status[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TRANSPARENT)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TRANSPARENT))) {
        action->hit = 1;
        g_battle_action_attacker_action_data.attack_type |= BATTLE_ACTION_TYPE_STATUS_CHANGE;
        g_battle_action_attacker_action_data.status_changes.removal[2]
            = g_battle_action_attacker_action_data.status_changes.removal[2]
            | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TRANSPARENT);
    }
}
