#include "fft/battle.h"
#include "psx/types.h"

/* Mirror of battle_move_has_reached_destination_tile_entry_edge against the tile the unit currently occupies rather
 * than its movement destination. */
s32 battle_move_has_reached_current_tile_exit_edge(s32 direction, battle_unit_misc_data_t* unit) {
    switch (direction) {
    case BATTLE_MOVE_DIRECTION_NORTH:
        return unit->screen.vz >= (((unit->map_y + 1) * 28) - 7);
    case BATTLE_MOVE_DIRECTION_SOUTH:
        return ((unit->map_y * 28) + 7) >= unit->screen.vz;
    case BATTLE_MOVE_DIRECTION_WEST:
        return ((unit->map_x * 28) + 7) >= unit->screen.vx;
    case BATTLE_MOVE_DIRECTION_EAST:
        return unit->screen.vx >= (((unit->map_x + 1) * 28) - 7);
    }
}
