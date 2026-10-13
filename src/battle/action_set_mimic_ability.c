#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Builds and previews a reaction using the current attacker's action and weapons.
 *
 * Returns 0 for an ineligible unit, -1 if targeting or preview fails, and 1
 * on success. Failed targeting or preview clears both borrowed weapon slots.
 * The menu type stays byte-sized, and the facing subtraction and action-field
 * assignments retain the target's evaluation order. The tile index and saved
 * action state share a temporary because their lifetimes do not overlap.
 */
s32 action_set_mimic_ability(unit_stats_t* unit) {
    /* A byte flag test retains the original compound-condition scheduling. */
    action_command_t action;
    action_command_t* current;
    s32 x;
    s32 y;
    s32 z;
    s32 direction;
    u8 menu_type;
    s32 result;
    s32 saved_state;
    s32 weapon_id;
    s32 unit_id;
    s32 attacker_facing;
    s32 ability_id;
    s32 facing;

    current = &g_action_current_attacker.action;
    if (action_is_reaction_prevented(unit) != 0) {
        return 0;
    }
    if ((unit->team_flags & BATTLE_TEAM_MASK) != g_action_current_attacker.team) {
        return 0;
    }
    if (unit->battle_id == current->actor_id) {
        return 0;
    }
    menu_type = g_main_action_menu_types_by_skillset[current->skillset_id];
    /* The target loads the ability id signed (lh). */
    ability_id = g_action_current_attacker.action.ability_id;
    if (menu_type == ACTION_MENU_TYPE_DEFAULT || menu_type == ACTION_MENU_TYPE_ARITHMETICKS
        || menu_type == ACTION_MENU_TYPE_MONSTER) {
        if (g_main_ability_range_data[ability_id].flags_3 & ABILITY_SECONDARY_FLAG_3_CANNOT_MIMIC) {
            return 0;
        }
    }
    facing = unit->position.facing;
    attacker_facing = g_action_current_attacker.facing - 4;
    direction = facing - attacker_facing;
    if (menu_type == ACTION_MENU_TYPE_JUMP) {
        direction |= 0x100;
    }
    if (map_resolve_rotated_offset_layer(unit, g_action_current_attacker.target_delta_x,
            g_action_current_attacker.target_delta_y, direction, &x, &y, &z)
        != 0) {
        return -1;
    }
    main_util_copy_action_data(current, &action);
    action.targeting_type = BATTLE_TARGETING_TYPE_TILE;
    action.actor_id = unit->battle_id;
    unit_id = unit->battle_id;
    action.target_position.vx = x;
    action.target_position.vy = z;
    weapon_id = g_action_current_attacker.right_hand_weapon_id;
    action.target_position.vz = y;
    action.target_id = unit_id;
    unit->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON] = weapon_id;
    unit->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON] = g_action_current_attacker.left_hand_weapon_id;
    result = target_set_panels_for_action(&action);
    if (result == -1 || result == 3
        || (result < 2
            && !((*(u8*)&g_map_tile_data[saved_state = (z << 8) + (g_map_max_x * y) + x].ceiling_depth_and_marks)
                & MAP_TILE_FLAG_ABILITY_RANGE))) {
        unit->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON] = ITEM_ID_NONE;
        unit->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON] = ITEM_ID_NONE;
        return -1;
    }
    saved_state = g_action_state;
    g_action_state = ACTION_STATE_PREVIEW;
    result = action_prepare_attack(&action, &unit->action_command, 1);
    unit->charged_ability_ct = 0xff;
    main_status_change_unit(unit, 0,
        STATUS_BYTE_MASK(STATUS_ID_CHARGING) | STATUS_BYTE_MASK(STATUS_ID_JUMP)
            | STATUS_BYTE_MASK(STATUS_ID_PERFORMING),
        MAIN_STATUS_REMOVE);
    g_action_state = saved_state;
    if (result < 0) {
        unit->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON] = ITEM_ID_NONE;
        unit->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON] = ITEM_ID_NONE;
        return -1;
    }
    return 1;
}
