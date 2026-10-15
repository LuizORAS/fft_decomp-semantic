#include "fft/battle.h"
#include "psx/types.h"

/* Return in `output` the depth-sort point of a unit stepping between tiles, for the current camera yaw:
 * the point moved half a tile (0xe) toward the corner the yaw quadrant picks and back by `offset` on x and z,
 * with y raised by 0xc. Callers pass 1, or 5 on a tile with MAP_TILE_SURFACE_FLAG_DEPTH_BIAS
 * (battle_camera_get_step_sort_point_unbiased and _biased). */
void battle_camera_get_step_sort_point(const u16* input, s16* output, s32 offset) {
    s32 x;
    s32 z;

    output[1] = input[1] - 0xc;
    switch (g_battle_camera_angles.vy & 0xc00) {
    case 0:
        x = input[0] + 0xe;
        output[0] = x - offset;
        z = input[2] - 0xe;
        output[2] = z + offset;
        break;
    case 0x400:
        x = input[0] + 0xe;
        output[0] = x - offset;
        z = input[2] + 0xe;
        output[2] = z - offset;
        break;
    case 0x800:
        x = input[0] - 0xe;
        output[0] = x + offset;
        z = input[2] + 0xe;
        output[2] = z - offset;
        break;
    case 0xc00:
        x = input[0] - 0xe;
        output[0] = x + offset;
        z = input[2] - 0xe;
        output[2] = z + offset;
        break;
    }
}
