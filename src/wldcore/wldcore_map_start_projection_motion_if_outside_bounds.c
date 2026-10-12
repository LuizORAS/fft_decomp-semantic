#include "fft/wldcore.h"
#include "psx/libgte.h"

/* Start a camera pan toward a map position when its screen point calls for one
 * (wldcore_map_is_point_outside_projection_bounds): from the current origin to the origin that
 * centres the position (wldcore_map_negate_and_clamp_coordinates), over a number of frames that grows
 * with the distance. Returns 1 when a pan started.
 *
 * The motion record snapshots the current origin and stores the target
 * distance in the same 8-bit fractional scale used by its progress field. */
s32 wldcore_map_start_projection_motion_if_outside_bounds(const wldcore_point32_t* point, const s16* coordinates) {
    s32 dx;
    s32 dy;

    if (wldcore_map_is_point_outside_projection_bounds(point) == 0) {
        return 0;
    }

    wldcore_map_negate_and_clamp_coordinates(coordinates, &g_wldcore_map_projection_motion.target);
    g_wldcore_map_projection_motion.flags |= WLDCORE_PROJECTION_MOTION_ACTIVE;
    g_wldcore_map_projection_motion.origin = g_wldcore_map_projection_origin;

    dx = g_wldcore_map_projection_motion.target.x - g_wldcore_map_projection_motion.origin.vx;
    dy = g_wldcore_map_projection_motion.target.y - g_wldcore_map_projection_motion.origin.vy;
    g_wldcore_map_projection_motion.progress = 0;
    g_wldcore_map_projection_motion.distance = csqrt(dx * dx + dy * dy) >> 8;
    return 1;
}
