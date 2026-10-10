#include "fft/battle.h"

/* Distance in tiles along the map axes (|dx| + |dy|), ignoring height. */
s32 battle_ai_calculate_distance_between_units(battle_stats_t* first, battle_stats_t* second) {
    s32 dx = first->x - second->x;
    s32 dy = first->position.bits.y - second->position.bits.y;
    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;
    return dx + dy;
}
