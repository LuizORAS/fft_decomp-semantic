#include "fft/battle_ai.h"

/* Return the direction from origin to target along the larger of the x and y offsets (+y is north,
 * +x east). An exact diagonal goes to north (+x+y), east (+x-y), south (-x-y) or west (-x+y), and
 * the same tile gives BATTLE_AI_DIRECTION_OVERLAP. Both point at battle_ai_coords_t bytes (x at 0, y
 * at 2). */
battle_ai_direction_e battle_ai_find_direction_of_target(
    const battle_ai_coords_t* target, const battle_ai_coords_t* origin) {
    s8 target_y = target->fields.y;
    s8 target_x = target->fields.x;
    s8 origin_x = origin->fields.x;
    s8 origin_y = origin->fields.y;
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
