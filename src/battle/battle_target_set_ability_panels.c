#include "fft/battle.h"
#include "psx/types.h"

/* Mark the tiles in range of the action's ability (its ability_secondary_data_t): spread its range
 * from the unit's tile on both layers, use the weapon's range for WEAPON_RANGE abilities, or take
 * every tile for range 0xff (no retail ability). Then drop the unit's own tile (CANNOT_TARGET_SELF),
 * keep only the unit's row and column (VERTICAL_FIXED) and, per square, only the layer within
 * `vertical` levels of the unit's height (VERTICAL_TOLERANCE), and clear untargetable tiles for
 * single-tile and DIRECT_TARGETING abilities. Returns the number of tiles flagged
 * MAP_TILE_FLAG_ABILITY_RANGE.
 *
 * The separate byte offset and register binding preserve the target's copy of the location index.
 * Direct typed indexing removes that copy and changes the later register allocation; the binding
 * emits no instructions. */
s32 battle_target_set_ability_panels(const u8* source) {
    battle_ai_command_action_t action;
    battle_stats_t* unit;
    ability_secondary_data_t* ability;
    battle_target_panel_t* panels;
    battle_target_panel_t* origin;
    battle_target_panel_t* seed;
    battle_target_panel_t* seed_upper;
    map_tile_t* tile;
    s32 location;
    s32 index;
    s32 elevation;
    /* Pin: unpinned, GCC computes the origin offset in $v0 instead of $v1. */
    register s32 origin_offset __asm__("$3");
    s32 i;
    u8 range;
    u8 aoe;
    u8 vertical;
    u8 flags_1;
    u8 flags_4;
    s32 x;
    u8 y;

    main_util_copy_action_data(source, (u8*)&action);
    unit = &g_battle_unit_stats[action.unit_id];
    ability = &g_main_ability_range_data[action.ability_id];
    range = ability->range;
    aoe = ability->aoe;
    flags_1 = ability->flags_1;
    vertical = ability->vertical;
    flags_4 = ability->flags_4;
    x = unit->x;
    y = unit->position.bits.y;
    location = battle_map_calculate_location(unit);
    tile = &g_battle_map_tile_data[location];
    elevation = tile->height * 2 + (tile->depth_half_height & 0x1f) + (tile->depth_half_height >> 5) * 2;
    panels = g_battle_target_panels;
    origin_offset = location * sizeof(battle_target_panel_t);
    origin = (battle_target_panel_t*)((s32)panels + origin_offset);
    index = y * g_battle_map_max_x + x;
    if (range == 0xff) {
        for (i = 0; i < MAP_TILE_SLOT_COUNT; i++) {
            panels[i].remaining_range = 1;
            panels[i].mark = 0;
        }
    } else {
        for (i = 0; i < MAP_TILE_SLOT_COUNT; i++) {
            panels[i].remaining_range = 0;
            panels[i].mark = 0;
        }
        seed = &g_battle_target_panels[index];
        seed_upper = &g_battle_target_panels[index + 0x100];
        seed->remaining_range = range + 1;
        seed_upper->remaining_range = range + 1;
        seed->mark = 1;
        if (flags_1 & ABILITY_SECONDARY_FLAG_1_WEAPON_RANGE) {
            battle_target_calculate_weapon_range(unit);
            seed->remaining_range = 0;
            seed_upper->remaining_range = 0;
        } else {
            battle_target_spread_panels(range, 0);
        }
    }
    if (flags_1 & ABILITY_SECONDARY_FLAG_1_CANNOT_TARGET_SELF) {
        origin->remaining_range = 0;
    }
    if (flags_1 & ABILITY_SECONDARY_FLAG_1_VERTICAL_FIXED) {
        location = x;
        battle_target_apply_vertical_fixed(location, y);
    }
    if (flags_1 & ABILITY_SECONDARY_FLAG_1_VERTICAL_TOLERANCE) {
        battle_target_apply_vertical_tolerance(elevation, vertical, 0);
    }
    if (aoe == 0 || (flags_4 & ABILITY_SECONDARY_FLAG_4_DIRECT_TARGETING)) {
        battle_target_clear_panels_on_untargetable_tiles();
    }
    if (flags_1 & (ABILITY_SECONDARY_FLAG_1_ALLY_UNIT_TILES | ABILITY_SECONDARY_FLAG_1_ENEMY_UNIT_TILES)) {
        battle_target_mark_unit_panels_by_team(unit, flags_1);
    }
    if (flags_1
        & (ABILITY_SECONDARY_FLAG_1_ALLY_UNIT_TILES | ABILITY_SECONDARY_FLAG_1_ENEMY_UNIT_TILES
            | ABILITY_SECONDARY_FLAG_1_VERTICAL_FIXED)) {
        return battle_target_set_ability_range_flags_from_marks();
    }
    return battle_target_set_ability_range_flags();
}
