#include "fft/battle_move.h"
#include "psx/types.h"

enum {
    TILE_CENTER_OFFSET = BATTLE_MAP_TILE_SIZE / 2,
};

/*
 * Snaps the unit's world/real coordinate on the axis of travel to the center
 * of its movement destination tile: south/north (0, 2) set Y, west/east (1, 3)
 * set X.
 */
void battle_move_snap_axis_to_destination_tile_center(s32 direction, battle_unit_t* unit) {
    s16 coordinate;

    switch (direction) {
    case BATTLE_MOVE_DIRECTION_NORTH:
    case BATTLE_MOVE_DIRECTION_SOUTH:
        coordinate = unit->movement.destination_y * BATTLE_MAP_TILE_SIZE + TILE_CENTER_OFFSET;
        unit->world_coords.vz = coordinate;
        unit->real.vz = coordinate << BATTLE_MOVE_REAL_COORD_SHIFT;
        return;
    case BATTLE_MOVE_DIRECTION_EAST:
    case BATTLE_MOVE_DIRECTION_WEST:
        coordinate = unit->movement.destination_x * BATTLE_MAP_TILE_SIZE + TILE_CENTER_OFFSET;
        unit->world_coords.vx = coordinate;
        unit->real.vx = coordinate << BATTLE_MOVE_REAL_COORD_SHIFT;
        return;
    }
}
