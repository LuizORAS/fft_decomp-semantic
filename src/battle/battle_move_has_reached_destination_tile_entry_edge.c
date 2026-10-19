#include "fft/battle_move.h"
#include "psx/types.h"

/* Whether a walking unit has reached the near edge of its movement destination
 * tile in the given direction.  Tiles are 28 world units wide and the test
 * keeps a 7-unit margin. */
s32 battle_move_has_reached_destination_tile_entry_edge(s32 direction, battle_unit_t* unit) {
    switch (direction) {
    case BATTLE_MOVE_DIRECTION_NORTH:
        return unit->world_coords.vz >= ((unit->movement.destination_y * BATTLE_MAP_TILE_SIZE) + 7);
    case BATTLE_MOVE_DIRECTION_SOUTH:
        return (((unit->movement.destination_y + 1) * BATTLE_MAP_TILE_SIZE) - 7) >= unit->world_coords.vz;
    case BATTLE_MOVE_DIRECTION_WEST:
        return (((unit->movement.destination_x + 1) * BATTLE_MAP_TILE_SIZE) - 7) >= unit->world_coords.vx;
    case BATTLE_MOVE_DIRECTION_EAST:
        return unit->world_coords.vx >= ((unit->movement.destination_x * BATTLE_MAP_TILE_SIZE) + 7);
    }
}
