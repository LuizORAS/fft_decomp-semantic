#include "fft/wldcore.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Project every location dot and hide the ones off screen (outside x -128..128, y -116..120), all
 * of them while MAIN_SYSTEM_FLAG_HIDE_MAP_DOTS is set, and the locations whose visibility variable
 * is 0. */
void wldcore_map_project_and_cull_dots(void) {
    VECTOR position;
    long flag;
    s32 i;

    for (i = 0; i < g_wldcore_map_dot_count; i++) {
        RotTrans(&g_wldcore_map_dots[i].position, &position, &flag);
        g_wldcore_map_dots[i].screen_x = position.vx;
        g_wldcore_map_dots[i].screen_y = position.vy;
        if (position.vx < -128 || position.vx > 128 || position.vy < -116 || position.vy > 120
            || (g_main_system_flags & MAIN_SYSTEM_FLAG_HIDE_MAP_DOTS)
            || world_script_get_variable(i + EVENT_SCRIPT_VAR_LOCATION_VISIBLE_FIRST) == 0) {
            g_wldcore_map_dots[i].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        } else {
            g_wldcore_map_dots[i].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        }
    }
}
