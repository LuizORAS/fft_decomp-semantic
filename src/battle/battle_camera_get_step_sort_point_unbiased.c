#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_camera_get_step_sort_point with offset 1: a stepping unit on a tile without
 * MAP_TILE_SURFACE_FLAG_DEPTH_BIAS. */
void battle_camera_get_step_sort_point_unbiased(const u16* input, s16* output) {
    battle_camera_get_step_sort_point(input, output, 1);
}
