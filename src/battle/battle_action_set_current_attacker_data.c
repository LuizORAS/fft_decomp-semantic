#include "fft/battle.h"
#include "psx/types.h"

/* Record the acting unit's action for Mimes to copy: the action itself, the
 * target's offset from the unit, the unit's facing, team and weapons.
 *
 * A Mime's own action is not recorded. For a tile target only the low byte of
 * each coordinate is used. */
void battle_action_set_current_attacker_data(battle_stats_t* unit) {
    battle_stats_t* target;
    u8 x;
    u8 y;

    if (unit->job_id == JOB_ID_MIME) {
        return;
    }
    main_util_copy_action_data(&unit->action_command, &g_battle_action_current_attacker.action);
    if (unit->action_command.targeting_type == BATTLE_ACTION_TARGET_UNIT) {
        target = &g_battle_unit_stats[unit->action_command.target_id];
        x = target->x;
        y = target->position.bits.y;
    } else {
        /* Only the low byte of the s16 target coordinates is read. */
        x = unit->action_command.target_x;
        y = unit->action_command.target_y;
    }
    g_battle_action_current_attacker.target_delta_x = x - unit->x;
    g_battle_action_current_attacker.target_delta_y = y - unit->position.bits.y;
    g_battle_action_current_attacker.facing = unit->position.bits.facing;
    g_battle_action_current_attacker.team = unit->team_flags & BATTLE_TEAM_MASK;
    g_battle_action_current_attacker.right_hand_weapon_id = unit->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON];
    g_battle_action_current_attacker.left_hand_weapon_id = unit->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON];
    g_battle_action_current_attacker.mimic_pending = 1;
}
