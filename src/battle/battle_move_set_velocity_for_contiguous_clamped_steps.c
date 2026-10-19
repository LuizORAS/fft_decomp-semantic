#include "fft/battle_map.h"
#include "fft/battle_move.h"
#include "psx/types.h"

enum {
    BATTLE_MOVE_STEP_HALF_WIDTH = BATTLE_MAP_TILE_SIZE / 2,
    BATTLE_MOVE_STEP_NEAR_EDGE = -7,
    BATTLE_MOVE_STEP_FAR_EDGE = 35,
};

/*
 * Builds the per-frame velocity for a run of identical movement steps: walks
 * the remaining path while the steps keep the same direction quadrant and the
 * clamped ground height stays at the unit's current height, then hands the
 * accumulated edge-to-edge delta to the velocity solver.
 * battle_move_set_velocity_for_contiguous_steps_with_final_tile_height does the
 * same, but aims the path's last step at the destination tile's surface.
 *
 * The step code is copied to a 16-bit value before the switch: read through the
 * byte again, cse reuses the direction test's load and the reload is lost.
 */
void battle_move_set_velocity_for_contiguous_clamped_steps(
    battle_unit_t* unit, const battle_walk_path_t* path, const battle_move_step_t* step) {
    VECTOR direction;
    const battle_move_step_t* current;
    s32 start_x;
    s32 start_y;
    s32 start_z;
    s32 index;
    s32 x;
    s32 y;
    s32 end_x;
    s32 end_y;
    s32 end_z;
    s16 code;

    battle_map_get_tile_data_pointer(unit->map_x, unit->map_y, unit->map_z);
    x = unit->map_x;
    y = unit->map_y;
    index = unit->movement_path_offset - 1;
    start_x = unit->world_coords.vx;
    start_y = unit->world_coords.vz;
    start_z = unit->world_coords.vy;

    while (index < path->count) {
        current = &path->data.steps[index];
        if (current->direction != step->direction) {
            break;
        }
        code = *(u8*)current;
        switch (code >> 6) {
        case BATTLE_MOVE_STEP_DIRECTION_EAST:
            x += (code & 3) + 1;
            end_x = (x * BATTLE_MAP_TILE_SIZE) + BATTLE_MOVE_STEP_NEAR_EDGE;
            end_y = (y * BATTLE_MAP_TILE_SIZE) + BATTLE_MOVE_STEP_HALF_WIDTH;
            break;
        case BATTLE_MOVE_STEP_DIRECTION_WEST:
            x -= (code & 3) + 1;
            end_x = (x * BATTLE_MAP_TILE_SIZE) + BATTLE_MOVE_STEP_FAR_EDGE;
            end_y = (y * BATTLE_MAP_TILE_SIZE) + BATTLE_MOVE_STEP_HALF_WIDTH;
            break;
        case BATTLE_MOVE_STEP_DIRECTION_SOUTH:
            y -= (code & 3) + 1;
            end_x = (x * BATTLE_MAP_TILE_SIZE) + BATTLE_MOVE_STEP_HALF_WIDTH;
            end_y = (y * BATTLE_MAP_TILE_SIZE) + BATTLE_MOVE_STEP_FAR_EDGE;
            break;
        case BATTLE_MOVE_STEP_DIRECTION_NORTH:
            y += (code & 3) + 1;
            end_x = (x * BATTLE_MAP_TILE_SIZE) + BATTLE_MOVE_STEP_HALF_WIDTH;
            end_y = (y * BATTLE_MAP_TILE_SIZE) + BATTLE_MOVE_STEP_NEAR_EDGE;
            break;
        }
        end_z = battle_move_clamp_z_to_tile_headroom(start_z, x, y, current->higher);
        if (end_z != start_z) {
            break;
        }
        index++;
    }

    direction.vx = end_x - start_x;
    direction.vz = end_y - start_y;
    direction.vy = end_z - start_z;
    VectorNormal(&direction, &unit->velocity);
    unit->velocity.vx = unit->velocity.vx * g_main_system_vsync_interval;
    unit->velocity.vy = unit->velocity.vy * g_main_system_vsync_interval;
    unit->velocity.vz = unit->velocity.vz * g_main_system_vsync_interval;
}
