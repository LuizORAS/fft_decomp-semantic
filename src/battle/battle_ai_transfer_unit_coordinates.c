#include "fft/battle.h"

/* Pack a unit's tile as AI coordinates: x, y and the elevation bit (position
 * bit 15). */
void battle_ai_transfer_unit_coordinates(s32 unit_idx, battle_ai_coords_t* dst) {
    dst->bytes.x = g_battle_unit_stats[unit_idx].x;
    dst->bytes.y = g_battle_unit_stats[unit_idx].position.raw;
    dst->bytes.elevation = g_battle_unit_stats[unit_idx].position.raw >> 15;
    dst->bytes.zero = 0;
}
