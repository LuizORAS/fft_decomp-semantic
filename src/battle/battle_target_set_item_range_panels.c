#include "fft/battle_map.h"
#include "fft/battle_target.h"
#include "psx/types.h"

#include "fft/battle.h"
/* Item and Throw: mark the tiles within `range` steps of the unit (bits 0-6; bit 7 leaves out the
 * unit's own tile), on both layers, except blocked and untargetable tiles. Returns their count. */
s32 battle_target_set_item_range_panels(const battle_stats_t* unit, u8 range) {
    /* A byte collision test retains the original single-mask branch. */
    battle_target_panel_t* panel;
    battle_target_panel_t* upper;
    battle_target_panel_t* panels;
    map_tile_t* tile;
    s32 index;
    s32 i;
    s32 count;
    u8 remaining;

    index = unit->position.y * g_battle_map_max_x + unit->x;
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
        if (panels->remaining_range != 0 && !(((*(u8*)&tile[i].flags) & MAP_TILE_COLLISION_MASK))) {
            tile[i].ceiling_depth_and_marks.ability_range = 1;
            count++;
        } else {
            tile[i].ceiling_depth_and_marks.ability_range = 0;
        }
        panels++;
    }
    return count;
}
