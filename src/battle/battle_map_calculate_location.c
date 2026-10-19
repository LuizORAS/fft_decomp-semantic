#include "fft/battle_map.h"
#include "psx/types.h"

/* Index of the unit's tile in g_battle_map_tile_data: layer * 256 + y * map width + x, from the
 * position in its battle record (the layer is bit 15 of the packed position). */
s32 battle_map_calculate_location(const battle_stats_t* unit) {
    u8 y = unit->position.y;
    u16 elevation = unit->position.higher_elevation;
    u8 x = unit->x;
    return (elevation << 8) + (y * g_battle_map_max_x) + x;
}
