#include "fft/battle.h"

/* Mark the area an ability hits (MAP_TILE_FLAG_TARGETED) and return the number of tiles. It centres
 * on the unit's own tile for linear and three-direction abilities, on the target unit for a unit
 * target (targeting type 6), else on the chosen tile; the aoe spreads from there (the whole map for
 * 0xff, the tile alone for WEAPON_RANGE and DIRECT_TARGETING abilities) within `vertical` levels of
 * its height (the upper layer only with UPPER_LAYER_ONLY), and Moldball Virus skips water. Then the
 * caster's tile drops out (CANNOT_HIT_CASTER), lines replace the area (LINEAR_ATTACK,
 * THREE_DIRECTIONS), and ally-only or enemy-only abilities drop the other team's units. Returns -1
 * for an off-map or blocked centre.
 *
 * `location` carries the column into the index sum: the extra copies give the index pseudo the
 * allocation priority the target needs (a2), while keeping the `(elevation * 256 + y * width) + x`
 * operand order. */
s32 battle_target_mark_ability_area(battle_ai_command_action_t* action) {
    battle_ai_command_action_t action_copy;
    ability_secondary_data_t* ability;
    battle_stats_t* unit;
    battle_stats_t* target;
    map_tile_t* tile;
    battle_target_panel_t* panel;
    battle_target_panel_t* other_panel;
    battle_target_panel_t* cursor;
    battle_target_panel_t* marked;
    s32 x;
    s32 y;
    s32 elevation;
    s32 index;
    s32 location;
    s32 height;
    s32 i;
    s32 count;
    u8 unit_id;
    u8 flags_1;
    u8 flags_2;
    u8 aoe;
    u8 vertical;

    main_util_copy_action_data((const u8*)action, (u8*)&action_copy);
    ability = &g_main_ability_range_data[action_copy.ability_id];
    flags_1 = ability->flags_1;
    flags_2 = ability->flags_2;
    aoe = ability->aoe;
    vertical = ability->vertical;
    unit_id = action_copy.unit_id;
    unit = &g_battle_unit_stats[unit_id];
    if (ability->flags_4 & ABILITY_SECONDARY_FLAG_4_DIRECT_TARGETING) {
        flags_1 |= ABILITY_SECONDARY_FLAG_1_WEAPON_RANGE;
        flags_2 &= (u8) ~(ABILITY_SECONDARY_FLAG_2_THREE_DIRECTIONS | ABILITY_SECONDARY_FLAG_2_LINEAR_ATTACK
            | ABILITY_SECONDARY_FLAG_2_CAN_TARGET_ENEMIES | ABILITY_SECONDARY_FLAG_2_CAN_TARGET_ALLIES);
    }
    if (flags_2 & (ABILITY_SECONDARY_FLAG_2_THREE_DIRECTIONS | ABILITY_SECONDARY_FLAG_2_LINEAR_ATTACK)) {
        x = unit->x;
        y = unit->position.bits.y;
        elevation = unit->position.bits.higher_elevation;
    } else if (action_copy.targeting_type != 6) {
        x = (u8)action_copy.target_x;
        y = (u8)action_copy.target_y;
        elevation = (u8)action_copy.target_elevation;
    } else {
        target = &g_battle_unit_stats[action_copy.target_id];
        x = target->x;
        y = target->position.bits.y;
        elevation = target->position.bits.higher_elevation;
    }
    if (x >= g_battle_map_max_x) {
        return -1;
    }
    if (y >= g_battle_map_max_y || elevation >= 2) {
        return -1;
    }
    location = x;
    index = location;
    index = elevation * 256 + y * g_battle_map_max_x + location;
    tile = &g_battle_map_tile_data[index];
    if (tile->flags_06.value & MAP_TILE_FLAG_BLOCKED) {
        return -1;
    }
    height = tile->height * 2 + (tile->depth_half_height & 0x1f) + (tile->depth_half_height >> 5) * 2;
    if (aoe == 0xff) {
        cursor = g_battle_target_panels;
        for (i = 0; i < 0x200; i++) {
            cursor[i].remaining_range = 1;
            cursor[i].mark = 0;
        }
    } else {
        cursor = g_battle_target_panels;
        for (i = 0; i < 0x200; i++) {
            cursor[i].remaining_range = 0;
            cursor[i].mark = 0;
        }
        panel = &g_battle_target_panels[index];
        panel->remaining_range = aoe + 1;
        if (index < 0x100) {
            other_panel = &g_battle_target_panels[index + 0x100];
            panel->mark = 1;
        } else {
            other_panel = &g_battle_target_panels[index - 0x100];
            other_panel->mark = 1;
        }
        g_battle_target_panels[index & 0xff].mark = 1;
        if (!(flags_1 & ABILITY_SECONDARY_FLAG_1_WEAPON_RANGE)) {
            panel->remaining_range = aoe + 1;
            other_panel->remaining_range = aoe + 1;
            battle_target_spread_panels(aoe, 0);
            /* UPPER_LAYER_ONLY, as 0 or 1. */
            battle_target_apply_vertical_tolerance(height, vertical, (flags_2 >> 5) & 1);
            battle_target_check_moldball_virus_depth(action_copy.ability_id);
        }
    }
    cursor = &g_battle_target_panels[battle_map_calculate_location(unit)];
    if (flags_2 & ABILITY_SECONDARY_FLAG_2_CANNOT_HIT_CASTER) {
        cursor->remaining_range = 0;
    }
    if (flags_2 & ABILITY_SECONDARY_FLAG_2_LINEAR_ATTACK) {
        battle_target_build_directional_attack_panels(&action_copy, 1);
    }
    if (flags_2 & ABILITY_SECONDARY_FLAG_2_THREE_DIRECTIONS) {
        battle_target_build_directional_attack_panels(&action_copy, 3);
    }
    if (flags_2 & (ABILITY_SECONDARY_FLAG_2_CAN_TARGET_ALLIES | ABILITY_SECONDARY_FLAG_2_CAN_TARGET_ENEMIES)) {
        battle_target_apply_unit_team_eligibility(unit_id, flags_2 & ABILITY_SECONDARY_FLAG_2_CAN_TARGET_ALLIES,
            flags_2 & ABILITY_SECONDARY_FLAG_2_CAN_TARGET_ENEMIES, aoe == 0xff);
    }
    count = 0;
    if (!(flags_2 & (ABILITY_SECONDARY_FLAG_2_CAN_TARGET_ALLIES | ABILITY_SECONDARY_FLAG_2_CAN_TARGET_ENEMIES))) {
        tile = g_battle_map_tile_data;
        for (i = 0, marked = g_battle_target_panels; i < 0x200; i++) {
            if (marked->remaining_range != 0 && !(tile[i].flags_06.value & MAP_TILE_COLLISION_MASK)) {
                count++;
                tile[i].ceiling_depth_and_marks |= MAP_TILE_FLAG_TARGETED;
            } else {
                tile[i].ceiling_depth_and_marks &= ~MAP_TILE_FLAG_TARGETED;
            }
            marked++;
        }
        return count;
    }
    tile = g_battle_map_tile_data;
    for (i = 0, marked = g_battle_target_panels; i < 0x200; i++) {
        if (marked->mark != 0 && !(tile[i].flags_06.value & MAP_TILE_COLLISION_MASK)) {
            count++;
            tile[i].ceiling_depth_and_marks |= MAP_TILE_FLAG_TARGETED;
        } else {
            tile[i].ceiling_depth_and_marks &= ~MAP_TILE_FLAG_TARGETED;
        }
        marked++;
    }
    return count;
}
