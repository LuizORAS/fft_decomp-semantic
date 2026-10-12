#include "fft/battle_ai.h"

/* Side of origin the target tile lies on, split along the two diagonals (south
 * is lower y, west lower x). A tile on a diagonal counts as south (to the
 * south-west), west (north-west), north (north-east) or east (south-east).
 * OVERLAP for the same tile. */
ai_direction_e ai_find_direction_of_target(const ai_coords_t* target, const ai_coords_t* origin) {
    s8 target_y = target->fields.y;
    s8 target_x = target->fields.x;
    s8 origin_x = origin->fields.x;
    s8 origin_y = origin->fields.y;
    s32 diagonal_y_minus_x = ((target_y - target_x) + origin_x) - origin_y;
    s32 diagonal_y_plus_x = ((target_y + target_x) - origin_x) - origin_y;

    if (diagonal_y_minus_x <= 0) {
        if (diagonal_y_plus_x < 0)
            return AI_DIRECTION_SOUTH;
    } else {
        if (diagonal_y_plus_x <= 0)
            return AI_DIRECTION_WEST;
    }
    if (diagonal_y_minus_x >= 0 && diagonal_y_plus_x > 0)
        return AI_DIRECTION_NORTH;
    if (diagonal_y_minus_x < 0 && diagonal_y_plus_x >= 0)
        return AI_DIRECTION_EAST;
    return AI_DIRECTION_OVERLAP;
}
