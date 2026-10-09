#include "fft/battle.h"
#include "psx/types.h"

void battle_action_set_current_attacker_data(battle_stats_t* unit) {
    battle_stats_t* target;
    u8 x;
    u8 y;

    if (unit->job_id == JOB_ID_MIME) {
        return;
    }
    main_util_copy_action_data(&unit->action_actor_id, &g_battle_action_current_attacker.action);
    if (unit->action_target_kind == BATTLE_ACTION_TARGET_UNIT) {
        target = &g_battle_unit_stats[unit->action_target_id];
        x = target->x;
        y = target->position.bits.y;
    } else {
        /* Only the low byte of the s16 target coordinates is read. */
        x = unit->action_target_x;
        y = unit->action_target_y;
    }
    g_battle_action_current_attacker.target_delta_x = x - unit->x;
    g_battle_action_current_attacker.target_delta_y = y - unit->position.bits.y;
    g_battle_action_current_attacker.facing = unit->position.bits.facing;
    g_battle_action_current_attacker.team = unit->team_flags & BATTLE_TEAM_MASK;
    g_battle_action_current_attacker.right_hand_weapon_id = unit->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON];
    g_battle_action_current_attacker.left_hand_weapon_id = unit->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON];
    g_battle_action_current_attacker.mimic_pending = 1;
}
