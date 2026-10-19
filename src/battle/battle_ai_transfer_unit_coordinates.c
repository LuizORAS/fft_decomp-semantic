#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/* Write battle unit unit_idx's tile as AI coordinates: x, y, the map level (bit 15 of its position) and a
 * zero byte. */
void battle_ai_transfer_unit_coordinates(s32 unit_idx, battle_ai_coords_t* dst) {
    dst->fields.x = g_battle_unit_stats[unit_idx].x;
    dst->fields.y = g_battle_unit_stats[unit_idx].position.y;
    dst->fields.elevation = g_battle_unit_stats[unit_idx].position.higher_elevation;
    dst->fields.zero = 0;
}
