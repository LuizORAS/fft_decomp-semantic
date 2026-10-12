#include "fft/battle_camera.h"

/* battle_camera_calculate_relative_offset with offset 1: the depth point of a stepping unit on a
 * tile without MAP_TILE_SURFACE_FLAG_DEPTH_BIAS. */
void battle_camera_calculate_relative_offset_by_1(const SVECTOR* input, SVECTOR* output) {
    battle_camera_calculate_relative_offset(input, output, 1);
}
