#include "fft/battle_move.h"
#include "psx/types.h"

enum { TILE_EDGE_MARGIN = 7, REAL_COORD_SHIFT = 12 };

/*
 * Snaps the unit's screen/real coordinate on the axis of travel to the margin
 * band of the tile it currently occupies, mirroring
 * battle_move_snap_axis_to_destination_tile_center against map_x/map_y.
 */
void battle_move_snap_axis_to_current_tile_exit_edge(s32 direction, battle_unit_misc_data_t* unit) {
    s16 coordinate;

    switch (direction) {
    case BATTLE_MOVE_DIRECTION_NORTH:
        coordinate = (unit->map_y + 1) * MAP_TILE_SIZE - TILE_EDGE_MARGIN;
        unit->screen.vz = coordinate;
        unit->real.vz = coordinate << REAL_COORD_SHIFT;
        return;
    case BATTLE_MOVE_DIRECTION_SOUTH:
        coordinate = unit->map_y * MAP_TILE_SIZE + TILE_EDGE_MARGIN;
        unit->screen.vz = coordinate;
        unit->real.vz = coordinate << REAL_COORD_SHIFT;
        return;
    case BATTLE_MOVE_DIRECTION_WEST:
        coordinate = unit->map_x * MAP_TILE_SIZE + TILE_EDGE_MARGIN;
        unit->screen.vx = coordinate;
        unit->real.vx = coordinate << REAL_COORD_SHIFT;
        return;
    case BATTLE_MOVE_DIRECTION_EAST:
        coordinate = (unit->map_x + 1) * MAP_TILE_SIZE - TILE_EDGE_MARGIN;
        unit->screen.vx = coordinate;
        unit->real.vx = coordinate << REAL_COORD_SHIFT;
        return;
    }
}
