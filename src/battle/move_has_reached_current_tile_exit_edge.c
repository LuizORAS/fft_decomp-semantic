#include "fft/battle_move.h"
#include "psx/types.h"

/* Mirror of move_has_reached_destination_tile_entry_edge against the tile the unit currently occupies rather
 * than its movement destination. */
s32 move_has_reached_current_tile_exit_edge(s32 direction, battle_unit_misc_data_t* unit) {
    switch (direction) {
    case MOVE_DIRECTION_NORTH:
        return unit->screen.vz >= (((unit->map_y + 1) * MAP_TILE_SIZE) - 7);
    case MOVE_DIRECTION_SOUTH:
        return ((unit->map_y * MAP_TILE_SIZE) + 7) >= unit->screen.vz;
    case MOVE_DIRECTION_WEST:
        return ((unit->map_x * MAP_TILE_SIZE) + 7) >= unit->screen.vx;
    case MOVE_DIRECTION_EAST:
        return unit->screen.vx >= (((unit->map_x + 1) * MAP_TILE_SIZE) - 7);
    }
}
