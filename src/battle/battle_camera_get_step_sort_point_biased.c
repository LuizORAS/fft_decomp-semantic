#include "fft/battle_camera.h"

/* Same as battle_camera_get_step_sort_point with offset 5: a stepping unit on a tile with
 * MAP_TILE_SURFACE_FLAG_DEPTH_BIAS. */
void battle_camera_get_step_sort_point_biased(const SVECTOR* input, SVECTOR* output) {
    battle_camera_get_step_sort_point(input, output, 5);
}
