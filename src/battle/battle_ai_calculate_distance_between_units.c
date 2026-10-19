#include "fft/battle_ai.h"

/* Return the Manhattan distance between two units' tiles (x and y only, no height or layer). */
s32 battle_ai_calculate_distance_between_units(const battle_stats_t* first, const battle_stats_t* second) {
    s32 dx = first->x - second->x;
    s32 dy = first->position.y - second->position.y;
    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;
    return dx + dy;
}
