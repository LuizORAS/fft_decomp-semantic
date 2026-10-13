#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/* Pack a unit's tile as AI coordinates: x, y and the elevation bit (position
 * bit 15). */
void ai_copy_unit_coordinates(s32 unit_idx, ai_coords_t* dst) {
    dst->fields.x = g_battle_unit_stats[unit_idx].x;
    dst->fields.y = g_battle_unit_stats[unit_idx].position.y;
    dst->fields.elevation = g_battle_unit_stats[unit_idx].position.higher_elevation;
    dst->fields.zero = 0;
}
