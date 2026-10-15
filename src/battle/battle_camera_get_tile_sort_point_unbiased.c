#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_camera_get_tile_sort_point with offset 1: a standing unit, a cursor or a bubble on a tile
 * without MAP_TILE_SURFACE_FLAG_DEPTH_BIAS. */
void battle_camera_get_tile_sort_point_unbiased(const s16* screen, s16* destination) {
    battle_camera_get_tile_sort_point(screen, destination, 1);
}
