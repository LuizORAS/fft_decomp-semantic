#include "fft/wldcore.h"

/* Empty the menu stack and push the root level with its cursor on the player's marker (4 pixels
 * above it). */
void wldcore_menu_reset_to_root_level(void) {
    g_wldcore_menu_stack_depth = 0;
    wldcore_menu_push_root_level();
    g_wldcore_window_records[g_wldcore_menu_stack_records[1].window_index].position.x
        = g_wldcore_map_projection_state.marker.render.position.x;
    g_wldcore_window_records[g_wldcore_menu_stack_records[1].window_index].position.y
        = g_wldcore_map_projection_state.marker.render.position.y - 4;
}
