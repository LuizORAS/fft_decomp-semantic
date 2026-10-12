#include "fft/battle_move.h"

/* Travel direction of the step in progress (move_direction_e), from the
 * unit's tile to its destination tile. A difference in y decides before one in x,
 * and the result is NORTH when the two tiles are the same. */
s32 move_get_direction(const battle_unit_misc_data_t* unit) {
    s32 direction;

    if (unit->map_y < unit->movement.destination_y) {
        direction = MOVE_DIRECTION_NORTH;
    } else if (unit->movement.destination_y < unit->map_y) {
        direction = MOVE_DIRECTION_SOUTH;
    } else if (unit->map_x > unit->movement.destination_x) {
        direction = MOVE_DIRECTION_WEST;
    } else if (unit->map_x < unit->movement.destination_x) {
        direction = MOVE_DIRECTION_EAST;
    } else {
        direction = MOVE_DIRECTION_NORTH;
    }
    return direction;
}
