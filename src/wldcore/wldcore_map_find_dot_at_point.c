#include "fft/wldcore.h"
#include "psx/types.h"

/* The location dot under a screen point, as a one-based index (0 for none): a visible, shown dot
 * whose screen position is within 10 pixels across and from 14 above to 6 below the point; of
 * several, the lowest priority value wins, the first on a tie (every dot has 0xf).
 *
 * The point is passed by value in a0/a1; the target reads both members back
 * from the incoming argument home slots. */
s32 wldcore_map_find_dot_at_point(wldcore_point32_t point) {
    s32 best;
    s32 i;
    s32 hit;
    s32 value;

    best = 0xFFFFFF;
    hit = 0;
    for (i = 0; i < g_wldcore_map_dot_count; i++) {
        if (world_script_get_variable(i + EVENT_SCRIPT_VAR_LOCATION_VISIBLE_FIRST) == 0) {
            continue;
        }
        if ((g_wldcore_map_dots[i].flags & (WLDCORE_DISPLAY_OBJECT_BLANK | WLDCORE_DISPLAY_OBJECT_HIDDEN)) != 0) {
            continue;
        }
        value = g_wldcore_map_dots[i].screen_x;
        if (point.x < value - 10 || value + 10 < point.x) {
            continue;
        }
        value = g_wldcore_map_dots[i].screen_y;
        if (point.y < value - 14 || value + 6 < point.y) {
            continue;
        }
        value = g_wldcore_map_dots[i].priority;
        if (value < best) {
            hit = i + 1;
            best = value;
        }
    }
    return hit;
}
