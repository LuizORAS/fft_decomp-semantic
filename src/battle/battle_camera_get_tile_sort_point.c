#include "fft/battle.h"
#include "psx/types.h"

/* Return in `output` the point a sprite standing on a tile is depth-sorted by, for the current camera yaw:
 * the point's tile, snapped to the 28-unit grid, at the corner the yaw quadrant picks, moved `offset` units
 * into the tile on x and z, with y raised by 0xb. The callers transform it (RotTrans): its depth picks the
 * ordering-table entry of a unit, shadow, cursor or bubble, and the targeting cursor takes its screen position
 * (battle_target_project_cursor_tile_to_screen). Callers pass 1, or 5 on a tile with
 * MAP_TILE_SURFACE_FLAG_DEPTH_BIAS
 * (battle_camera_get_tile_sort_point_unbiased and _biased).
 *
 * Same shape as battle_camera_get_step_sort_point: each of the x and z
 * components picks up the caller's offset with a sign that depends on the yaw
 * quadrant, and the y component is a plain bias. Here each component is first
 * snapped down to a whole tile, and on the side the camera faces away from it
 * is moved on by one further tile. The far_edge temporaries keep the target's
 * add-then-subtract order; written inline, GCC re-associates them into a
 * longer sequence. */
void battle_camera_get_tile_sort_point(const s16* input, s16* output, s32 offset) {
    s32 yaw;

    output[1] = input[1] - 0xb;
    yaw = g_battle_camera_angles.vy & 0xc00;

    switch (yaw) {
    case 0: {
        s32 far_edge = (input[0] / 28) * 28 + 0x1c;
        output[0] = far_edge - offset;
    }
        output[2] = (input[2] / 28) * 28 + offset;
        break;
    case 0x400: {
        s32 far_edge = (input[0] / 28) * 28 + 0x1c;
        output[0] = far_edge - offset;
    }
        {
            s32 far_edge = (input[2] / 28) * 28 + 0x1c;
            output[2] = far_edge - offset;
        }
        break;
    case 0x800:
        output[0] = (input[0] / 28) * 28 + offset;
        {
            s32 far_edge = (input[2] / 28) * 28 + 0x1c;
            output[2] = far_edge - offset;
        }
        break;
    case 0xc00:
        output[0] = (input[0] / 28) * 28 + offset;
        output[2] = (input[2] / 28) * 28 + offset;
        break;
    }
}
