#include "fft/battle_camera.h"
#include "psx/types.h"

/* Return in `output` the depth-sort point of a unit stepping between tiles, for the current camera yaw:
 * the point moved half a tile (0xe) toward the corner the yaw quadrant picks and back by `offset` on x and z,
 * with y raised by 0xc. Callers pass 1, or 5 on a tile with MAP_TILE_SURFACE_FLAG_DEPTH_BIAS
 * (battle_camera_get_step_sort_point_unbiased and _biased). */
void battle_camera_get_step_sort_point(const SVECTOR* input, SVECTOR* output, s32 offset) {
    u16 x;
    u16 z;

    output->vy = input->vy - 0xc;
    switch (g_battle_camera_angles.vy & 0xc00) {
    case 0:
        x = input->vx + 0xe;
        output->vx = x - offset;
        z = input->vz - 0xe;
        output->vz = z + offset;
        break;
    case 0x400:
        x = input->vx + 0xe;
        output->vx = x - offset;
        z = input->vz + 0xe;
        output->vz = z - offset;
        break;
    case 0x800:
        x = input->vx - 0xe;
        output->vx = x + offset;
        z = input->vz + 0xe;
        output->vz = z - offset;
        break;
    case 0xc00:
        x = input->vx - 0xe;
        output->vx = x + offset;
        z = input->vz - 0xe;
        output->vz = z + offset;
        break;
    }
}
