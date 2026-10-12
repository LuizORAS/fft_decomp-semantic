#include "fft/battle_move.h"
#include "psx/types.h"

/* Whether a walking unit has reached the near edge of its movement destination
 * tile in the given direction.  Tiles are 28 screen units wide and the test
 * keeps a 7-unit margin. */
s32 move_has_reached_destination_tile_entry_edge(s32 direction, battle_unit_misc_data_t* unit) {
    switch (direction) {
    case MOVE_DIRECTION_NORTH:
        return unit->screen.vz >= ((unit->movement.destination_y * MAP_TILE_SIZE) + 7);
    case MOVE_DIRECTION_SOUTH:
        return (((unit->movement.destination_y + 1) * MAP_TILE_SIZE) - 7) >= unit->screen.vz;
    case MOVE_DIRECTION_WEST:
        return (((unit->movement.destination_x + 1) * MAP_TILE_SIZE) - 7) >= unit->screen.vx;
    case MOVE_DIRECTION_EAST:
        return unit->screen.vx >= ((unit->movement.destination_x * MAP_TILE_SIZE) + 7);
    }
}
