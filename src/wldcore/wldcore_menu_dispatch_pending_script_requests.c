#include "fft/wldcore.h"

/* Run the current location's pending map requests (its WLDCORE_SCRIPT_ACTION_MAP_MENU_MASK
 * conditions): draw or erase a route (the map path level), show or hide a location (the location
 * visibility level), or focus the camera on a location (0xff: the current one). Each resets the
 * current level's window first. The root level runs them when it resumes and a visit when it
 * starts.
 *
 * The flag tests are signed `> 0` comparisons; `!= 0` folds to srl/andi. */
void wldcore_menu_dispatch_pending_script_requests(void) {
    s32* location = &g_wldcore_map_projection_state.marker.kind;
    s32* flags;
    s32 id;

    if (wldcore_script_process_conditional_set(*location, WLDCORE_SCRIPT_ACTION_MAP_MENU_MASK) == 0) {
        return;
    }
    flags = &g_wldcore_script_state.flags;
    if (*flags & WLDCORE_SCRIPT_ACTION_PATH_MASK) {
        wldcore_window_reset_state_and_scroll(&g_wldcore_menu_stack_records[g_wldcore_menu_stack_depth]);
        wldcore_menu_push_map_path_level(g_wldcore_script_state.args[0], g_wldcore_script_state.args[1],
            (*flags & WLDCORE_SCRIPT_ACTION_ERASE_PATH) > 0);
    } else if (*flags & WLDCORE_SCRIPT_ACTION_LOCATION_VISIBILITY_MASK) {
        wldcore_window_reset_state_and_scroll(&g_wldcore_menu_stack_records[g_wldcore_menu_stack_depth]);
        wldcore_menu_push_map_location_visibility_level(
            g_wldcore_script_state.args[0], (*flags & WLDCORE_SCRIPT_ACTION_ERASE_LOCATION) > 0);
    } else if (*flags & WLDCORE_SCRIPT_ACTION_FOCUS_LOCATION) {
        wldcore_window_reset_state_and_scroll(&g_wldcore_menu_stack_records[g_wldcore_menu_stack_depth]);
        id = g_wldcore_script_state.args[0];
        if (id == 0xFF) {
            id = *location;
        }
        wldcore_menu_push_focus_location_level(id);
    }
}
