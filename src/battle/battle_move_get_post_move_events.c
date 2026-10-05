#include "fft/battle.h"
#include "psx/types.h"

/* Build the post-move events (BATTLE_MOVE_POST_EVENT_*) of a unit that has just moved: a crystal or
 * treasure chest on its tile; for a rider, the statuses that mounting removes from its mount
 * (MAIN_STATUS_CHECK_SET_MOUNT_REMOVAL, applied through the action target); a found item when the
 * unit (a rider: its mount) has Move-Find Item and the tile holds one, else the tile's trap; a
 * charged action the move cancels; and its movement abilities
 * (battle_move_get_movement_ability_events). */
u32 battle_move_get_post_move_events(battle_stats_t* unit) {
    battle_stats_t* target;
    u32 flags;
    s32 i;
    s32 find_item;
    battle_move_find_result_data_t* tile;

    /* Synthesize CRYSTAL_OR_TREASURE when the filtered tile search returns a
     * non-negative unit ID. The explicit sign test keeps the target nor/srl. */
    flags = (u32)~battle_unit_find_at_tile(unit->x, unit->position.bits.y, unit->position.bits.higher_elevation,
                BATTLE_UNIT_TILE_FILTER_CRYSTAL | BATTLE_UNIT_TILE_FILTER_TREASURE)
        >> 31;
    if (!(unit->mount_info & BATTLE_MOUNT_INFO_FLAG_RIDER)) {
        target = unit;
    } else {
        target = &g_battle_unit_stats[unit->mount_info & BATTLE_MOUNT_INFO_PARTNER_ID_MASK];
        g_battle_action_target = target;
        g_battle_action_target_data = &target->action;
        battle_action_clear_current_data(&target->action);
        for (i = 0; i < BATTLE_STATUS_BYTE_COUNT; i++) {
            g_battle_action_target_data->status_removal[i]
                = g_main_status_check_sets[MAIN_STATUS_CHECK_SET_MOUNT_REMOVAL][i];
        }
        if (battle_status_modify_inflictions(0) != 0) {
            g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_STATUS_CHANGE;
            battle_status_resolve_unit_changes(target->misc_unit_id, 0);
            flags |= BATTLE_MOVE_POST_EVENT_MOUNT_STATUS_CHANGE;
        }
    }
    find_item = target->movement_abilities[2] & BATTLE_MOVEMENT_SET_3_MOVE_FIND_ITEM;
    tile = battle_map_get_move_find_result(unit->x, unit->position.bits.y, unit->position.bits.higher_elevation);
    if (find_item && (tile->flags & BATTLE_MOVE_FIND_RESULT_ITEM)) {
        flags |= BATTLE_MOVE_POST_EVENT_ITEM_FOUND;
    } else if (tile->flags & BATTLE_MOVE_FIND_RESULT_TRAP) {
        flags |= BATTLE_MOVE_POST_EVENT_TRAP_TRIGGERED;
    }
    if (battle_status_remove_charging_ability_ct(unit, 0) != 0) {
        flags |= BATTLE_MOVE_POST_EVENT_CHARGING_CANCEL;
    }
    flags |= battle_move_get_movement_ability_events(unit);
    return flags;
}
