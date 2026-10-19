#include "fft/battle_ai.h"
#include "fft/battle_map.h"

/* Return 1 when a unit on source should not face target, the tile next to it: off the map, blocked,
 * or at least 3 higher (the lower layer's height against the source's own layer). Descents, slopes and
 * depth do not count. */
s32 battle_ai_is_facing_tile_blocked(const battle_ai_coords_t* source, const battle_ai_coords_t* target) {
    u8 x = target->fields.x;
    u8 width = g_battle_ai_data_base.map_max_x;
    u8 y;
    map_tile_t* tile;

    if (x >= width) {
        return 1;
    }
    y = target->fields.y;
    if (y >= g_battle_ai_data_base.map_max_y || (tile = &g_battle_map_tile_data[y * width + x])->flags.blocked) {
        return 1;
    }
    return tile->height
        - g_battle_map_tile_data[(source->fields.elevation * MAP_TILES_PER_LAYER) + source->fields.y * width
            + source->fields.x]
              .height
        >= 3;
}
