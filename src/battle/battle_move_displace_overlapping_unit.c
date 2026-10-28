#include "fft/battle_move.h"
#include "psx/types.h"

/* Keep a unit that shares the mover's tile pushed aside while the mover leaves it: the push
 * (battle_move_displace_unit_along_step_direction, in the direction
 * battle_move_displace_unit_at_destination_tile chose) shrinks from 10 units at the center to 0 once
 * the mover is 7 units past it. Skipped on the first step and when the mover came onto this tile by
 * a step onto a unit (BATTLE_MOVE_STEP_ON_UNIT).
 *
 * The case order and per-case center temporaries reproduce the target's cross-jumped tails; indexing
 * from movement_path - 2 reproduces its offset-before-base address add. */
void battle_move_displace_overlapping_unit(battle_unit_t* unit, s32 direction) {
    battle_unit_t* other;
    s32 dist;
    s32 x;
    s32 y;
    s32 center;

    other = battle_unit_find_overlapping_at_tile(unit, unit->map_x, unit->map_y, unit->map_z);
    if (unit->movement_path_offset < 2) {
        return;
    }
    if (unit->path.data.steps[unit->movement_path_offset - 2].mount) {
        return;
    }
    if (other == 0) {
        return;
    }
    x = other->map_x * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
    y = other->map_y * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
    switch (direction) {
    case BATTLE_MOVE_DIRECTION_NORTH:
        center = unit->map_y * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
        dist = unit->world_coords.vz - center;
        break;
    case BATTLE_MOVE_DIRECTION_SOUTH:
        center = unit->map_y * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
        dist = center - unit->world_coords.vz;
        break;
    case BATTLE_MOVE_DIRECTION_WEST:
        center = unit->map_x * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
        dist = center - unit->world_coords.vx;
        break;
    case BATTLE_MOVE_DIRECTION_EAST:
        center = unit->map_x * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
        dist = unit->world_coords.vx - center;
        break;
    }
    if (dist >= 8) {
        dist = 7;
    }
    dist -= 7;
    battle_move_displace_unit_along_step_direction(other, x, y, -dist * 3 / 2);
}
