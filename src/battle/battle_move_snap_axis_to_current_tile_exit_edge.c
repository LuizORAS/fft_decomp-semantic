#include "fft/battle_move.h"
#include "psx/types.h"

enum { TILE_EDGE_MARGIN = 7 };

/*
 * Snaps the unit's world/real coordinate on the axis of travel to the margin
 * band of the tile it currently occupies, mirroring
 * battle_move_snap_axis_to_destination_tile_center against map_x/map_y.
 */
void battle_move_snap_axis_to_current_tile_exit_edge(s32 direction, battle_unit_t* unit) {
    s16 coordinate;

    switch (direction) {
    case BATTLE_MOVE_DIRECTION_NORTH:
        coordinate = (unit->map_y + 1) * BATTLE_MAP_TILE_SIZE - TILE_EDGE_MARGIN;
        unit->world_coords.vz = coordinate;
        unit->real.vz = coordinate << BATTLE_MOVE_REAL_COORD_SHIFT;
        return;
    case BATTLE_MOVE_DIRECTION_SOUTH:
        coordinate = unit->map_y * BATTLE_MAP_TILE_SIZE + TILE_EDGE_MARGIN;
        unit->world_coords.vz = coordinate;
        unit->real.vz = coordinate << BATTLE_MOVE_REAL_COORD_SHIFT;
        return;
    case BATTLE_MOVE_DIRECTION_WEST:
        coordinate = unit->map_x * BATTLE_MAP_TILE_SIZE + TILE_EDGE_MARGIN;
        unit->world_coords.vx = coordinate;
        unit->real.vx = coordinate << BATTLE_MOVE_REAL_COORD_SHIFT;
        return;
    case BATTLE_MOVE_DIRECTION_EAST:
        coordinate = (unit->map_x + 1) * BATTLE_MAP_TILE_SIZE - TILE_EDGE_MARGIN;
        unit->world_coords.vx = coordinate;
        unit->real.vx = coordinate << BATTLE_MOVE_REAL_COORD_SHIFT;
        return;
    }
}
