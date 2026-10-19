#include "fft/battle_ai.h"
#include "fft/battle_target.h"
#include "fft/battle_unit.h"

/* Fill tile_target_distance with each tile's movement cost from target as the acting unit moves
 * (battle_ai_propagate_target_movement spreads from target); an unmounted Teleport or Teleport 2
 * mover uses the Manhattan distance. Only rows holding scenario-0 reachable tiles are filled, across
 * the map width. Returns -1 when the propagation suspends, else 0. */
s32 battle_ai_fill_target_distance_grid(const battle_ai_coords_t* target) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 suspended = 0;
    s32 use_manhattan;
    s32 target_remaining_range;
    s32 level, x, y;
    s32 distance, y_distance;

    if (g_battle_ai_data_base.resume_pending != 0)
        g_battle_ai_data_base.search_resume_flag = 0;
    else
        g_battle_ai_data_base.search_resume_flag = 1;
    use_manhattan = 0;
    if (!(ai->acting_unit->mount_info & BATTLE_MOUNT_INFO_FLAG_RIDER)
        && (ai->acting_unit->ability_flags.movement[1]
            & (BATTLE_MOVEMENT_SET_2_TELEPORT | BATTLE_MOVEMENT_SET_2_TELEPORT_2))) {
        use_manhattan = 1;
    } else {
        battle_ai_propagate_target_movement(ai->acting_unit_id, target->fields.x, target->fields.y,
            target->fields.elevation, ai->search_resume_flag, &suspended, 1);
        if (suspended != 0)
            return -1;
        ai->resume_pending = 0;
        target_remaining_range = g_battle_target_panels[(target->fields.elevation * MAP_TILES_PER_LAYER)
            + target->fields.y * ai->map_max_x + target->fields.x]
                                     .remaining_range;
    }
    for (level = 0; level < MAP_TERRAIN_LAYER_COUNT; level++) {
        for (y = 0; y < ai->map_max_y; y++) {
            if (ai->reachable_tiles[0][level][y] != 0) {
                for (x = 0; x < ai->map_max_x; x++) {
                    if (use_manhattan) {
                        distance = target->fields.x - x;
                        if (distance < 0)
                            distance = -distance;
                        y_distance = target->fields.y - y;
                        if (y_distance < 0)
                            y_distance = -y_distance;
                        distance += y_distance;
                    } else {
                        distance = target_remaining_range
                            - g_battle_target_panels[(level * MAP_TILES_PER_LAYER) + y * ai->map_max_x + x]
                                  .remaining_range;
                    }
                    ai->tile_target_distance[level][y][x] = distance;
                }
            }
        }
    }
    return 0;
}
