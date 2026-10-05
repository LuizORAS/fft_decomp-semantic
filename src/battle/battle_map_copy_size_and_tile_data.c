#include "fft/battle.h"
#include "psx/libc.h"
#include "psx/types.h"

/* Load the map's terrain from its GNS terrain resource (0x1a): the width and depth into
 * g_battle_map_max_x and g_battle_map_max_y, then 0x1000 bytes of tiles, 256 eight-byte map_tile_t
 * per layer, into g_battle_map_tile_data. */
void battle_map_copy_size_and_tile_data(u8* data) {
    g_battle_map_max_x = *data++;
    g_battle_map_max_y = *data++;
    memcpy((u8*)g_battle_map_tile_data, data, 0x1000);
}
