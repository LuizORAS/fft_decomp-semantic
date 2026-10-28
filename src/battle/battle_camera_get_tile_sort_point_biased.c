#include "fft/battle_camera.h"

/* Same as battle_camera_get_tile_sort_point with offset 5: on a tile with
 * MAP_TILE_SURFACE_FLAG_DEPTH_BIAS. */
void battle_camera_get_tile_sort_point_biased(const SVECTOR* screen, SVECTOR* destination) {
    battle_camera_get_tile_sort_point(screen, destination, 5);
}
