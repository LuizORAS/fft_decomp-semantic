#include "fft/battle.h"
#include "psx/types.h"

enum {
    TILE_SIZE_UNITS = 0x1c,
    TILE_CENTER_OFFSET = 0xe,
    REAL_COORD_SHIFT = 12,
};

/*
 * Snaps the unit's screen/real coordinate on the axis of travel to the centre
 * of its movement destination tile: south/north (0, 2) set Y, west/east (1, 3)
 * set X.
 */
/* Target 0x8006ca3c. */
void battle_move_snap_axis_to_destination_tile_center(s32 direction, battle_unit_misc_data_t* unit) {
    s16 coordinate;

    switch (direction) {
    case BATTLE_MOVE_DIRECTION_NORTH:
    case BATTLE_MOVE_DIRECTION_SOUTH:
        coordinate = unit->movement.bytes.destination_y * TILE_SIZE_UNITS + TILE_CENTER_OFFSET;
        unit->screen.vz = coordinate;
        unit->real.vz = coordinate << REAL_COORD_SHIFT;
        return;
    case BATTLE_MOVE_DIRECTION_EAST:
    case BATTLE_MOVE_DIRECTION_WEST:
        coordinate = unit->movement.bytes.destination_x * TILE_SIZE_UNITS + TILE_CENTER_OFFSET;
        unit->screen.vx = coordinate;
        unit->real.vx = coordinate << REAL_COORD_SHIFT;
        return;
    }
}
