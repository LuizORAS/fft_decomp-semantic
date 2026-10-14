#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_camera_get_tile_sort_point with offset 5: on a tile with
 * MAP_TILE_SURFACE_FLAG_DEPTH_BIAS. */
void battle_camera_get_tile_sort_point_biased(const s16* screen, s16* destination) {
    battle_camera_get_tile_sort_point(screen, destination, 5);
}
