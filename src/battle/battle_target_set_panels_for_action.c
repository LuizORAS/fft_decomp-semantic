/*
 * The .ld places .rodata at 0x80174068 so the switch's jump table lands on the
 * target's table.
 */
#include "fft/battle.h"
#include "psx/types.h"

/* Build the targeting panels for an action and report how its target is chosen.
 *
 * The skillset's menu type picks the range. Abilities (the default and monster
 * menus, the Elemental ability of the unit's tile, the Draw Out of the chosen
 * katana) use their range data through battle_target_set_ability_panels. Item
 * reaches 1 tile, or 4 with Throw Item or for a Mime. Throw reaches the unit's
 * Move and excludes its own tile. Attack and Charge use the weapon's range, Jump
 * the unit's Jump abilities.
 *
 * Returns a battle_target_panels_e value: INVALID for an absent unit, a katana or
 * ability id out of range or a menu type without an action; NO_TARGETING for the
 * remaining menu types and for an auto ability with range 0 that does not use the
 * weapon's range; NONE when no tile is marked; TILE_ONLY when the action cannot
 * follow a unit (always for Item, Throw, Attack, Charge and Jump); otherwise
 * UNIT_OR_TILE. */
s32 battle_target_set_panels_for_action(u8* source) {
    battle_ai_command_action_t action;
    battle_stats_t* unit;
    ability_secondary_data_t* ability;
    s32 mode;
    s32 range;
    s32 result;
    u8 flags;
    u16 ability_id;

    main_util_copy_action_data(source, &action);
    mode = 0;
    if (action.unit_id >= BATTLE_UNIT_SLOT_COUNT) {
        return BATTLE_TARGET_PANELS_INVALID;
    }
    unit = &g_battle_unit_stats[action.unit_id];
    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
        return BATTLE_TARGET_PANELS_INVALID;
    }
    switch (g_main_action_menu_types_by_skillset[action.skillset]) {
    case ACTION_MENU_TYPE_DEFAULT:
    case ACTION_MENU_TYPE_MONSTER:
        ability_id = action.ability_id;
        mode = 1;
        break;
    case ACTION_MENU_TYPE_ITEM_INVENTORY:
        range = ((unit->support_abilities[2] & BATTLE_SUPPORT_SET_3_THROW_ITEM) || unit->job_id == JOB_ID_MIME) ? 4 : 1;
        mode = 2;
        break;
    case ACTION_MENU_TYPE_WEAPON_INVENTORY:
        mode = 2;
        range = unit->move | 0x80;
        break;
    case ACTION_MENU_TYPE_ELEMENTS:
        ability_id = g_main_terrain_geomancy_abilities
            [g_battle_map_tile_data[battle_map_calculate_location(unit)].surface.value & MAP_SURFACE_MASK];
        mode = 1;
        break;
    case ACTION_MENU_TYPE_KATANA_INVENTORY:
        mode = 1;
        if (action.item_id < ITEM_ID_ASURA_KNIFE || action.item_id > ITEM_ID_CHIRIJIRADEN) {
            return BATTLE_TARGET_PANELS_INVALID;
        }
        ability_id = action.item_id + (ABILITY_ID_DRAW_OUT_ASURA - ITEM_ID_ASURA_KNIFE);
        break;
    case ACTION_MENU_TYPE_ATTACK:
    case ACTION_MENU_TYPE_CHARGE:
        result = battle_target_set_weapon_attack_panels(&action);
        flags = ABILITY_SECONDARY_FLAG_2_NO_FOLLOW_TARGET;
        break;
    case ACTION_MENU_TYPE_JUMP:
        result = battle_target_set_jump_ability_panels((const u8*)&action);
        flags = ABILITY_SECONDARY_FLAG_2_NO_FOLLOW_TARGET;
        break;
    case ACTION_MENU_TYPE_BLANK_05:
    case ACTION_MENU_TYPE_UNKNOWN_0F:
        return BATTLE_TARGET_PANELS_INVALID;
    default:
        return BATTLE_TARGET_PANELS_NO_TARGETING;
    }
    if (mode == 1) {
        if (ability_id >= ABILITY_ID_ITEM_FIRST) {
            return BATTLE_TARGET_PANELS_INVALID;
        }
        ability = &g_main_ability_range_data[ability_id];
        range = ability->range;
        flags = ability->flags_2;
        if ((ability->flags_1 & ABILITY_SECONDARY_FLAG_1_AUTO) && range == 0
            && !(ability->flags_1 & ABILITY_SECONDARY_FLAG_1_WEAPON_RANGE)) {
            return BATTLE_TARGET_PANELS_NO_TARGETING;
        }
        action.ability_id = ability_id;
        result = battle_target_set_ability_panels((const u8*)&action);
    }
    if (mode == 2) {
        result = battle_target_calculate_ability_range_with_map_parameters(unit, range & 0xff);
        flags = ABILITY_SECONDARY_FLAG_2_NO_FOLLOW_TARGET;
    }
    if (result != 0) {
        /* The named temporary is load-bearing; the direct test compiles differently. */
        s32 flag_result = flags & ABILITY_SECONDARY_FLAG_2_NO_FOLLOW_TARGET;

        return flag_result != 0;
    }
    return BATTLE_TARGET_PANELS_NONE;
}
