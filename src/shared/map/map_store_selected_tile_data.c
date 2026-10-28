#include "fft/battle_map.h"
#include "psx/types.h"

/* Fill g_map_selected_tile_data for the tile under the map cursor, which the tile info window
 * shows: surface type, displayed height (height * 2 + slope half height + depth * 2, in half levels)
 * and depth, plus the surface type's two g_main_terrain_tile_info_pairs bytes, which nothing reads.
 * WORLD's copy is world_map_store_selected_tile_data (0x800f2ee0). */
void map_store_selected_tile_data(void) {
    SVECTOR coords;
    s16* surface_type;
    s32 depth;

    battle_map_store_selected_tile_coordinates(&coords);
    surface_type = &g_map_selected_tile_data.surface_type;
    *surface_type = battle_map_get_tile_data_value(MAP_TILE_DATA_SURFACE_TYPE, coords.vx, coords.vz, coords.vy) & 0xFF;
    g_map_selected_tile_data.display_height
        = battle_map_get_tile_data_value(MAP_TILE_DATA_HALF_HEIGHT, coords.vx, coords.vz, coords.vy) & 0xFF;
    g_map_selected_tile_data.display_height
        += (battle_map_get_tile_data_value(MAP_TILE_DATA_HEIGHT, coords.vx, coords.vz, coords.vy) & 0xFF) * 2;
    depth = battle_map_get_tile_data_value(MAP_TILE_DATA_DEPTH, coords.vx, coords.vz, coords.vy) & 0xFF;
    g_map_selected_tile_data.depth = depth;
    g_map_selected_tile_data.display_height += depth * 2;
    g_map_selected_tile_data._unknown_02 = g_main_terrain_tile_info_pairs[*surface_type][0];
    g_map_selected_tile_data._unknown_04 = g_main_terrain_tile_info_pairs[*surface_type][1];
}
