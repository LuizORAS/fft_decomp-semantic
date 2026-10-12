#include "fft/battle_action.h"
#include "fft/battle_ai.h"
#include "fft/battle_map.h"

/* Intersect attack-origin tiles with the current ability's range.
 *
 * The acting unit's coordinates are restored before every return. */
s32 ai_calculate_ability_range(void) {
    ai_data_t* ai = &g_ai_data_base;
    map_tile_t* tile;
    s32 count;
    s32 level;
    s32 y;
    s32 x;
    u16* target_rows;
    u16* origin_rows;
    u16 row;
    s32 tile_mask;
    u32 level_address;
    char unused[24];

    ai_move_temp_unit_to_coords(&ai->current_action.coords);
    count = 0;
    if (ai->considered_ability.parameters.bytes.range != 0) {
        if (target_set_panels_for_action(&ai->current_action.command) == TARGET_PANELS_NONE) {
            ai_move_temp_unit_to_coords(&ai->acting_unit_coords);
            return 0;
        }
    } else {
        x = 0;
        tile = g_battle_map_tile_data;
        do {
            x++;
            tile->ceiling_depth_and_marks.ability_range = 0;
            tile++;
        } while (x < MAP_TILE_COUNT);
        g_battle_map_tile_data[ai->current_action.coords.bytes.y * ai->map_max_x + ai->current_action.coords.bytes.x
            + (ai->current_action.coords.bytes.elevation * MAP_TILES_PER_LAYER)]
            .ceiling_depth_and_marks.ability_range = 1;
    }

    level = 0;
    for (level_address = (u32)ai; level < MAP_TERRAIN_LAYER_COUNT;
        level++, level_address += AI_GRID_ROW_COUNT * sizeof(u16)) {
        for (y = 0; y < ai->map_max_y; y++) {
            target_rows = (u16*)(level_address + ((u32)&ai->targetable_tiles - (u32)ai));
            origin_rows = (u16*)(level_address + ((u32)&ai->attack_origin_tiles - (u32)ai));
            target_rows[y] = 0;
            row = origin_rows[y];
            if (row != 0) {
                for (x = 0; x < ai->map_max_x; x++) {
                    tile_mask = row << x;
                    if (tile_mask & 0x8000) {
                        if (g_battle_map_tile_data[y * ai->map_max_x + x + (level * MAP_TILES_PER_LAYER)]
                                .ceiling_depth_and_marks.ability_range) {
                            count++;
                            tile_mask = 0x8000;
                            target_rows[y] |= tile_mask >> x;
                        }
                    }
                }
            }
        }
    }
    ai_move_temp_unit_to_coords(&ai->acting_unit_coords);
    return count;
}
