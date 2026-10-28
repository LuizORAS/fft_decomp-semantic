#include "fft/battle_ai.h"
#include "fft/battle_map.h"
#include "fft/battle_target.h"

/* Put into targetable_tiles the target tiles in the considered ability's range from the current
 * action's coords (battle_target_set_panels_for_action, or that tile alone without range) and return
 * their count, 0 when none is in range. battle_ai_select_random_action picks among them; the
 * simulation unit returns to its own tile. */
s32 battle_ai_count_target_tiles_in_range(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
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
    char unused[24]; /* dead local kept for the target's frame size */

    battle_ai_move_temp_unit_to_coords(&ai->current_action.coords);
    count = 0;
    if (ai->considered_ability.parameters.fields.range != 0) {
        if (battle_target_set_panels_for_action(&ai->current_action.command) == BATTLE_TARGET_PANELS_NONE) {
            battle_ai_move_temp_unit_to_coords(&ai->acting_unit_coords);
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
        g_battle_map_tile_data[ai->current_action.coords.fields.y * ai->map_max_x + ai->current_action.coords.fields.x
            + (ai->current_action.coords.fields.elevation * MAP_TILES_PER_LAYER)]
            .ceiling_depth_and_marks.ability_range = 1;
    }

    level = 0;
    /* Direct typed row pointers change the original loop bytes. */
    for (level_address = (u32)ai; level < MAP_TERRAIN_LAYER_COUNT;
        level++, level_address += BATTLE_AI_GRID_ROW_COUNT * sizeof(u16)) {
        for (y = 0; y < ai->map_max_y; y++) {
            target_rows = (u16*)(level_address + ((u32)&ai->targetable_tiles - (u32)ai));
            origin_rows = (u16*)(level_address + ((u32)&ai->target_tiles - (u32)ai));
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
    battle_ai_move_temp_unit_to_coords(&ai->acting_unit_coords);
    return count;
}
