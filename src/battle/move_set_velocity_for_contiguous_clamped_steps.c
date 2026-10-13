#include "fft/battle_map.h"
#include "fft/battle_move.h"
#include "psx/types.h"

enum {
    MOVE_STEP_HALF_WIDTH = MAP_TILE_SIZE / 2,
    MOVE_STEP_NEAR_EDGE = -7,
    MOVE_STEP_FAR_EDGE = 35,
};

/*
 * Builds the per-frame velocity for a run of identical movement steps: walks
 * the remaining path while the steps keep the same direction quadrant and the
 * clamped ground height stays at the unit's current height, then hands the
 * accumulated edge-to-edge delta to the velocity solver.
 * move_set_velocity_for_contiguous_steps_with_final_tile_height does
 * the same for the final step, where it reads the tile directly.
 *
 * The step code is copied to a 16-bit value before the switch: read through the
 * byte again, cse reuses the direction test's load and the reload is lost.
 */
void move_set_velocity_for_contiguous_clamped_steps(
    battle_unit_t* unit, const battle_walk_path_t* path, const move_step_t* step) {
    VECTOR direction;
    const move_step_t* current;
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

    map_get_tile_data_pointer(unit->map_x, unit->map_y, unit->map_z);
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
        case MOVE_STEP_DIRECTION_EAST:
            x += (code & 3) + 1;
            end_x = (x * MAP_TILE_SIZE) + MOVE_STEP_NEAR_EDGE;
            end_y = (y * MAP_TILE_SIZE) + MOVE_STEP_HALF_WIDTH;
            break;
        case MOVE_STEP_DIRECTION_WEST:
            x -= (code & 3) + 1;
            end_x = (x * MAP_TILE_SIZE) + MOVE_STEP_FAR_EDGE;
            end_y = (y * MAP_TILE_SIZE) + MOVE_STEP_HALF_WIDTH;
            break;
        case MOVE_STEP_DIRECTION_SOUTH:
            y -= (code & 3) + 1;
            end_x = (x * MAP_TILE_SIZE) + MOVE_STEP_HALF_WIDTH;
            end_y = (y * MAP_TILE_SIZE) + MOVE_STEP_FAR_EDGE;
            break;
        case MOVE_STEP_DIRECTION_NORTH:
            y += (code & 3) + 1;
            end_x = (x * MAP_TILE_SIZE) + MOVE_STEP_HALF_WIDTH;
            end_y = (y * MAP_TILE_SIZE) + MOVE_STEP_NEAR_EDGE;
            break;
        }
        end_z = move_clamp_z_to_tile_headroom(start_z, x, y, current->higher);
        if (end_z != start_z) {
            break;
        }
        index++;
    }

    direction.vx = end_x - start_x;
    direction.vz = end_y - start_y;
    direction.vy = end_z - start_z;
    VectorNormal(&direction, &unit->velocity);
    unit->velocity.vx = unit->velocity.vx * g_main_system_animation_speed;
    unit->velocity.vy = unit->velocity.vy * g_main_system_animation_speed;
    unit->velocity.vz = unit->velocity.vz * g_main_system_animation_speed;
}
