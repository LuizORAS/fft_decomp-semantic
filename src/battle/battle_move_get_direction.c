#include "fft/battle_move.h"

/* Return the direction of the unit's next tile (battle_move_direction_e): SOUTH (0) for -y, EAST
 * (1) for +x, NORTH (2) for +y, WEST (3) for -x, a difference in y deciding first; NORTH also
 * when the unit is already there. */
battle_move_direction_e battle_move_get_direction(const battle_unit_t* unit) {
    battle_move_direction_e direction;

    if (unit->map_y < unit->movement.destination_y) {
        direction = BATTLE_MOVE_DIRECTION_NORTH;
    } else if (unit->movement.destination_y < unit->map_y) {
        direction = BATTLE_MOVE_DIRECTION_SOUTH;
    } else if (unit->map_x > unit->movement.destination_x) {
        direction = BATTLE_MOVE_DIRECTION_WEST;
    } else if (unit->map_x < unit->movement.destination_x) {
        direction = BATTLE_MOVE_DIRECTION_EAST;
    } else {
        direction = BATTLE_MOVE_DIRECTION_NORTH;
    }
    return direction;
}
