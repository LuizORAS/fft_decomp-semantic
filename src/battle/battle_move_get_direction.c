#include "fft/battle.h"

s32 battle_move_get_direction(const battle_unit_misc_data_t* unit) {
    s32 direction;

    if (unit->map_y < unit->movement.bytes.destination_y) {
        direction = BATTLE_MOVE_DIRECTION_NORTH;
    } else if (unit->movement.bytes.destination_y < unit->map_y) {
        direction = BATTLE_MOVE_DIRECTION_SOUTH;
    } else if (unit->map_x > unit->movement.bytes.destination_x) {
        direction = BATTLE_MOVE_DIRECTION_WEST;
    } else if (unit->map_x < unit->movement.bytes.destination_x) {
        direction = BATTLE_MOVE_DIRECTION_EAST;
    } else {
        direction = BATTLE_MOVE_DIRECTION_NORTH;
    }
    return direction;
}
