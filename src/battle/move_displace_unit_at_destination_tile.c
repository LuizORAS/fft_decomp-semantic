#include "fft/battle_move.h"
#include "psx/types.h"

/* Pushes a unit sharing the mover's destination tile aside, mirroring
 * move_displace_overlapping_unit but keyed on the pending step's
 * destination tile instead of the unit's current map square.  The trailing
 * loop only advances the shared direction counter g_move_displacement_direction; it has no other
 * effect.
 *
 * The case order and the reused `dist` temporary reproduce the target's
 * cross-jumped tails and its s1 allocation; spelling the second path index as
 * offset + base reproduces the offset-before-base address add. */
void move_displace_unit_at_destination_tile(battle_unit_t* unit, s32 direction) {
    battle_unit_t* other;
    s32 dist;
    s32 x;
    s32 y;
    s32 center;
    s32 next;

    if (unit->movement_path_offset == unit->path.count) {
        return;
    }
    other = battle_unit_find_overlapping_at_tile(
        unit, unit->movement.destination_x, unit->movement.destination_y, unit->movement.destination_z);
    if (unit->movement_path_offset == 0) {
        return;
    }
    if (unit->path.data.steps[unit->movement_path_offset - 1].mount) {
        return;
    }
    if (other == 0) {
        return;
    }
    x = other->movement.destination_x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    y = other->movement.destination_y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    switch (direction) {
    case MOVE_DIRECTION_NORTH:
        center = unit->movement.destination_y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
        dist = center - unit->world_coords.vz;
        break;
    case MOVE_DIRECTION_SOUTH:
        center = unit->movement.destination_y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
        dist = unit->world_coords.vz - center;
        break;
    case MOVE_DIRECTION_WEST:
        center = unit->movement.destination_x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
        dist = unit->world_coords.vx - center;
        break;
    case MOVE_DIRECTION_EAST:
        center = unit->movement.destination_x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
        dist = center - unit->world_coords.vx;
        break;
    }
    if (dist >= 8) {
        dist = 7;
    }
    dist -= 7;
    dist = -dist * 3 / 2;
    switch ((unit->movement_path_offset + unit->path.data.steps)->direction) {
    case MOVE_STEP_DIRECTION_EAST:
        next = MOVE_DIRECTION_EAST;
        break;
    case MOVE_STEP_DIRECTION_WEST:
        next = MOVE_DIRECTION_WEST;
        break;
    case MOVE_STEP_DIRECTION_SOUTH:
        next = MOVE_DIRECTION_SOUTH;
        break;
    case MOVE_STEP_DIRECTION_NORTH:
        next = MOVE_DIRECTION_NORTH;
        break;
    }
    for (g_move_displacement_direction = 0; g_move_displacement_direction < 4; g_move_displacement_direction++) {
        if (direction == next) {
            if ((g_move_displacement_direction != next) && (g_move_displacement_direction != (next ^ 2))) {
                break;
            }
        } else {
            if ((g_move_displacement_direction != (direction ^ 2)) && (g_move_displacement_direction != (next ^ 2))) {
                break;
            }
        }
    }
    move_displace_unit_along_step_direction(other, x, y, dist);
}
