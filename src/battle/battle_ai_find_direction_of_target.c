#include "fft/battle.h"

/* Side of origin the target tile lies on, split along the two diagonals (south
 * is lower y, west lower x). A tile on a diagonal counts as south (to the
 * south-west), west (north-west), north (north-east) or east (south-east).
 * OVERLAP for the same tile. */
battle_ai_direction_e battle_ai_find_direction_of_target(
    const battle_ai_coords_t* target, const battle_ai_coords_t* origin) {
    s8 target_y = target->bytes.y;
    s8 target_x = target->bytes.x;
    s8 origin_x = origin->bytes.x;
    s8 origin_y = origin->bytes.y;
    s32 diagonal_y_minus_x = ((target_y - target_x) + origin_x) - origin_y;
    s32 diagonal_y_plus_x = ((target_y + target_x) - origin_x) - origin_y;

    if (diagonal_y_minus_x <= 0) {
        if (diagonal_y_plus_x < 0)
            return BATTLE_AI_DIRECTION_SOUTH;
    } else {
        if (diagonal_y_plus_x <= 0)
            return BATTLE_AI_DIRECTION_WEST;
    }
    if (diagonal_y_minus_x >= 0 && diagonal_y_plus_x > 0)
        return BATTLE_AI_DIRECTION_NORTH;
    if (diagonal_y_minus_x < 0 && diagonal_y_plus_x >= 0)
        return BATTLE_AI_DIRECTION_EAST;
    return BATTLE_AI_DIRECTION_OVERLAP;
}
