#include "fft/battle_move.h"
#include "psx/types.h"

/*
 * Report whether a walking unit has reached the centre of its destination tile
 * along the given facing.  Tile centres are 28 screen units apart with a
 * half-tile offset of 14.
 */
s32 move_has_reached_destination_tile_center(s32 direction, battle_unit_misc_data_t* unit) {
    switch (direction) {
    case MOVE_DIRECTION_NORTH:
        return unit->screen.vz >= unit->movement.destination_y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    case MOVE_DIRECTION_SOUTH:
        return unit->movement.destination_y * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2) >= unit->screen.vz;
    case MOVE_DIRECTION_WEST:
        return unit->movement.destination_x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2) >= unit->screen.vx;
    case MOVE_DIRECTION_EAST:
        return unit->screen.vx >= unit->movement.destination_x * MAP_TILE_SIZE + (MAP_TILE_SIZE / 2);
    }
}
