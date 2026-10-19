#include "fft/wldcore.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Second names for three fields of g_wldcore_map_projection_motion: the
 * target loads each through its own address, in this order. */
extern s32 g_wldcore_map_projection_motion_progress;
extern s32 g_wldcore_map_projection_motion_origin_x;
extern s32 g_wldcore_map_projection_motion_target_x;

/* Step the camera pan one frame: the origin moves from where the pan started toward its target by
 * progress / distance. Returns 1 while the pan runs, and 0 on the frame it lands, with the origin
 * on the target and the pan's active flag cleared.
 *
 * The final snap goes through the VECTOR view of the target, so the four 32-bit values
 * copy as one struct assignment. */
s32 wldcore_map_step_projection_motion(void) {
    s32 progress;
    s32 distance;
    s32 target_x;
    s32 origin_x;
    s32 delta_x;
    s32 origin_y;

    target_x = g_wldcore_map_projection_motion_target_x;
    origin_x = g_wldcore_map_projection_motion_origin_x;
    progress = g_wldcore_map_projection_motion_progress;
    distance = g_wldcore_map_projection_motion.distance;
    target_x = (target_x - origin_x) * progress;
    delta_x = target_x / distance;
    g_wldcore_map_projection_origin.vx = origin_x + delta_x;
    origin_y = g_wldcore_map_projection_motion.origin.vy;
    g_wldcore_map_projection_origin.vy
        = origin_y + (g_wldcore_map_projection_motion.target.vy - origin_y) * progress / distance;
    if (progress >= distance) {
        g_wldcore_map_projection_origin = g_wldcore_map_projection_motion.target;
        g_wldcore_map_projection_motion.flags ^= WLDCORE_PROJECTION_MOTION_ACTIVE;
        return 0;
    }
    g_wldcore_map_projection_motion.progress = progress + 1;
    return 1;
}
