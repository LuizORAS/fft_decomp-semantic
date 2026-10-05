#include "fft/battle.h"
#include "psx/types.h"

/* Index of the unit's tile in g_battle_map_tile_data: layer * 256 + y * map width + x, from the
 * position in its battle record. */
s32 battle_map_calculate_location(battle_stats_t* unit) {
    u8 y = unit->position.bits.y;
    u16 y_elev = unit->position.raw;
    u8 x = unit->x;
    return ((y_elev >> 15) << 8) + (y * g_battle_map_max_x) + x;
}
