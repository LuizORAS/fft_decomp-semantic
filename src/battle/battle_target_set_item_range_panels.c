#include "fft/battle.h"
#include "psx/types.h"

/* Item and Throw: mark the tiles within `range` steps of the unit (bits 0-6; bit 7 leaves out the
 * unit's own tile), on both layers, except blocked and untargetable tiles. Returns their count. */
s32 battle_target_set_item_range_panels(battle_stats_t* unit, u8 range) {
    battle_target_panel_t* panel;
    battle_target_panel_t* upper;
    battle_target_panel_t* panels;
    map_tile_t* tile;
    s32 index;
    s32 i;
    s32 count;
    u8 remaining;

    index = unit->position.bits.y * g_battle_map_max_x + unit->x;
    battle_target_clear_panel_data();
    remaining = (range & 0x7f) + 1;
    panel = &g_battle_target_panels[index];
    upper = &g_battle_target_panels[index + 0x100];
    panel->remaining_range = remaining;
    upper->remaining_range = remaining;
    panel->mark = 1;
    battle_target_spread_panels(range, 0);
    if (range & 0x80) {
        panel->remaining_range = 0;
        upper->remaining_range = 0;
    }
    count = 0;
    tile = g_battle_map_tile_data;
    for (i = 0, panels = g_battle_target_panels; i < MAP_TILE_SLOT_COUNT; i++) {
        if (panels->remaining_range != 0 && !(tile[i].flags_06.value & MAP_TILE_COLLISION_MASK)) {
            tile[i].ceiling_depth_and_marks |= MAP_TILE_FLAG_ABILITY_RANGE;
            count++;
        } else {
            tile[i].ceiling_depth_and_marks &= ~MAP_TILE_FLAG_ABILITY_RANGE;
        }
        panels++;
    }
    return count;
}
