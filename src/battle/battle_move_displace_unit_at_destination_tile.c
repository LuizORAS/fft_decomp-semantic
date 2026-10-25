#include "fft/battle_move.h"
#include "psx/types.h"

/* Push aside a unit standing on the tile the mover is stepping onto: within 7 units of that tile's
 * center the occupant moves off it by (7 - distance) * 3 / 2, up to 10 units at the center
 * (battle_move_displace_unit_along_step_direction). Skipped on the last path step and on a step onto
 * a unit (BATTLE_MOVE_STEP_ON_UNIT).
 *
 * It also chooses the push direction, g_battle_move_displacement_direction, which
 * battle_move_displace_overlapping_unit reuses while the mover leaves: on a straight path the first
 * direction (by index) across it; on a turn the first that is neither the side the mover comes from
 * nor opposite the next step.
 *
 * The case order and the reused `dist` temporary reproduce the target's cross-jumped tails and its s1
 * allocation; spelling the second path index as offset + base reproduces the offset-before-base
 * address add. */
void battle_move_displace_unit_at_destination_tile(battle_unit_t* unit, s32 direction) {
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
    x = other->movement.destination_x * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
    y = other->movement.destination_y * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
    switch (direction) {
    case BATTLE_MOVE_DIRECTION_NORTH:
        center = unit->movement.destination_y * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
        dist = center - unit->world_coords.vz;
        break;
    case BATTLE_MOVE_DIRECTION_SOUTH:
        center = unit->movement.destination_y * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
        dist = unit->world_coords.vz - center;
        break;
    case BATTLE_MOVE_DIRECTION_WEST:
        center = unit->movement.destination_x * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
        dist = unit->world_coords.vx - center;
        break;
    case BATTLE_MOVE_DIRECTION_EAST:
        center = unit->movement.destination_x * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
        dist = center - unit->world_coords.vx;
        break;
    }
    if (dist >= 8) {
        dist = 7;
    }
    dist -= 7;
    dist = -dist * 3 / 2;
    switch ((unit->movement_path_offset + unit->path.data.steps)->direction) {
    case BATTLE_MOVE_STEP_DIRECTION_EAST:
        next = BATTLE_MOVE_DIRECTION_EAST;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_WEST:
        next = BATTLE_MOVE_DIRECTION_WEST;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_SOUTH:
        next = BATTLE_MOVE_DIRECTION_SOUTH;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_NORTH:
        next = BATTLE_MOVE_DIRECTION_NORTH;
        break;
    }
    for (g_battle_move_displacement_direction = 0; g_battle_move_displacement_direction < 4;
        g_battle_move_displacement_direction++) {
        if (direction == next) {
            if ((g_battle_move_displacement_direction != next)
                && (g_battle_move_displacement_direction != (next ^ 2))) {
                break;
            }
        } else {
            if ((g_battle_move_displacement_direction != (direction ^ 2))
                && (g_battle_move_displacement_direction != (next ^ 2))) {
                break;
            }
        }
    }
    battle_move_displace_unit_along_step_direction(other, x, y, dist);
}
