#include "fft/battle_camera.h"
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
void battle_camera_get_tile_sort_point(const SVECTOR* input, SVECTOR* output, s32 offset) {
    s32 yaw;

    output->vy = input->vy - 0xb;
    yaw = g_battle_camera_angles.vy & 0xc00;

    switch (yaw) {
    case 0: {
        s32 far_edge = (input->vx / BATTLE_MAP_TILE_SIZE) * BATTLE_MAP_TILE_SIZE + BATTLE_MAP_TILE_SIZE;
        output->vx = far_edge - offset;
    }
        output->vz = (input->vz / BATTLE_MAP_TILE_SIZE) * BATTLE_MAP_TILE_SIZE + offset;
        break;
    case 0x400: {
        s32 far_edge = (input->vx / BATTLE_MAP_TILE_SIZE) * BATTLE_MAP_TILE_SIZE + BATTLE_MAP_TILE_SIZE;
        output->vx = far_edge - offset;
    }
        {
            s32 far_edge = (input->vz / BATTLE_MAP_TILE_SIZE) * BATTLE_MAP_TILE_SIZE + BATTLE_MAP_TILE_SIZE;
            output->vz = far_edge - offset;
        }
        break;
    case 0x800:
        output->vx = (input->vx / BATTLE_MAP_TILE_SIZE) * BATTLE_MAP_TILE_SIZE + offset;
        {
            s32 far_edge = (input->vz / BATTLE_MAP_TILE_SIZE) * BATTLE_MAP_TILE_SIZE + BATTLE_MAP_TILE_SIZE;
            output->vz = far_edge - offset;
        }
        break;
    case 0xc00:
        output->vx = (input->vx / BATTLE_MAP_TILE_SIZE) * BATTLE_MAP_TILE_SIZE + offset;
        output->vz = (input->vz / BATTLE_MAP_TILE_SIZE) * BATTLE_MAP_TILE_SIZE + offset;
        break;
    }
}
