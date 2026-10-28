#include "fft/wldcore.h"
#include "psx/libgte.h"

/* Keep the traveling marker in view: when its next screen position (marker + delta) would leave
 * x -64..72 or y -64..64 in the direction it moves, scroll the map by the delta within the scroll
 * limits, and set MAIN_SYSTEM_FLAG_DRAW_MAP when the origin changed. */
void wldcore_map_scroll_projection_if_focus_near_edge(open_point32_t* delta) {
    open_point32_t position;
    VECTOR saved;

    saved = g_wldcore_map_projection_origin;
    position.x = g_wldcore_map_projection_state.marker.render.position.x + delta->x;
    position.y = g_wldcore_map_projection_state.marker.render.position.y + delta->y;
    if ((delta->x < 0 && position.x < -0x40) || (delta->x > 0 && position.x >= 0x49)) {
        wldcore_map_apply_clamped_horizontal_scroll(&delta->x, &g_wldcore_map_projection_origin.vx);
    }
    if ((delta->y < 0 && position.y < -0x40) || (delta->y > 0 && position.y >= 0x41)) {
        wldcore_map_apply_clamped_vertical_scroll(delta, &g_wldcore_map_projection_origin);
    }
    if (saved.vx != g_wldcore_map_projection_origin.vx || saved.vy != g_wldcore_map_projection_origin.vy) {
        g_main_system_flags |= MAIN_SYSTEM_FLAG_DRAW_MAP;
    }
}
