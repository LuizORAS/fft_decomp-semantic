#include "fft/battle_move.h"
#include "psx/types.h"

/* Mirror of move_has_reached_destination_tile_entry_edge against the tile the unit currently occupies rather
 * than its movement destination. */
s32 move_has_reached_current_tile_exit_edge(s32 direction, battle_unit_t* unit) {
    switch (direction) {
    case MOVE_DIRECTION_NORTH:
        return unit->world_coords.vz >= (((unit->map_y + 1) * MAP_TILE_SIZE) - 7);
    case MOVE_DIRECTION_SOUTH:
        return ((unit->map_y * MAP_TILE_SIZE) + 7) >= unit->world_coords.vz;
    case MOVE_DIRECTION_WEST:
        return ((unit->map_x * MAP_TILE_SIZE) + 7) >= unit->world_coords.vx;
    case MOVE_DIRECTION_EAST:
        return unit->world_coords.vx >= (((unit->map_x + 1) * MAP_TILE_SIZE) - 7);
    }
}
