#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/* Target 0x800f2ee0; byte-identical twin of battle_map_store_selected_tile_data. */
void world_map_store_selected_tile_data(void) {
    SVECTOR coords;
    s16* selected;
    s32 depth;

    battle_map_store_selected_tile_coordinates(&coords);
    selected = &g_world_map_selected_tile_data.surface_type;
    *selected = battle_map_get_tile_data_value(MAP_TILE_DATA_SURFACE_TYPE, coords.vx, coords.vz, coords.vy) & 0xFF;
    g_world_map_selected_tile_data.display_height
        = battle_map_get_tile_data_value(MAP_TILE_DATA_HALF_HEIGHT, coords.vx, coords.vz, coords.vy) & 0xFF;
    g_world_map_selected_tile_data.display_height
        += (battle_map_get_tile_data_value(MAP_TILE_DATA_HEIGHT, coords.vx, coords.vz, coords.vy) & 0xFF) * 2;
    depth = battle_map_get_tile_data_value(MAP_TILE_DATA_DEPTH, coords.vx, coords.vz, coords.vy) & 0xFF;
    g_world_map_selected_tile_data.depth = depth;
    g_world_map_selected_tile_data.display_height += depth * 2;
    g_world_map_selected_tile_data._unknown_02 = g_main_terrain_tile_info_pairs[*selected][0];
    g_world_map_selected_tile_data._unknown_04 = g_main_terrain_tile_info_pairs[*selected][1];
}
