#include "fft/script.h"
#include "fft/wldcore.h"

/* Start travelling from the current location to another: the map cursor's Circle, or a destination
 * chosen in WORLD's location list. When the current location's script starts an event now (its
 * WLDCORE_SCRIPT_ACTION_LOAD_EVENT condition) that event plays instead, through the event transition,
 * and 0 is returned. Otherwise its pending SetVar2 is applied and the best path is searched
 * (wldcore_map_find_best_route_path); with one, its first leg is staged, the camera pans to the
 * marker if needed, the destination's picture loads, and the cursor window and the location label
 * hide while wldcore_map_step_route_travel moves the marker. Returns 1.
 *
 * g_wldcore_route_search_depth is a separate scalar: as a field of the travel state it becomes the
 * address anchor that the later &g_wldcore_map_travel argument is formed from, and the chained 99999
 * store makes best_cost that anchor as in the target. The location load and the two argument
 * addresses share one base register in the target, which needs them to be members of the one
 * g_wldcore_map_projection_state view. */
s32 wldcore_map_start_travel(s32 from_location, s32 to_location) {
    s32 depth;

    if (wldcore_script_find_location_request(
            g_wldcore_map_projection_state.marker.kind, WLDCORE_SCRIPT_ACTION_LOAD_EVENT)
        != 0) {
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1].window_index].flags
            |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_next_map_id[0] = 0;
        script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, g_wldcore_script_state.args[0]);
        wldcore_menu_push_event_transition_level(g_wldcore_script_state.args[1]);
        g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL;
        return 0;
    } else {
        if (wldcore_script_find_location_request(
                g_wldcore_map_projection_state.marker.kind, WLDCORE_SCRIPT_ACTION_SET_VAR_2)
            != 0) {
            script_set_variable(g_wldcore_script_state.args[0], g_wldcore_script_state.args[1]);
        }
        g_wldcore_route_search_cost = 0;
        g_wldcore_route_search_depth = 0;
        g_wldcore_map_travel.best_cost = g_wldcore_map_travel.best_length_bound = 99999;
        g_wldcore_map_travel.flags = 0;
        wldcore_map_find_best_route_path(from_location, to_location);
        if (!(g_wldcore_map_travel.flags & WLDCORE_ROUTE_TRAVEL_ACTIVE)) {
            return 1;
        }
        g_wldcore_map_travel.route_index = 0;
        if (g_wldcore_map_travel.best_path_length == 0) {
            return 1;
        }
        g_wldcore_map_travel.segment_count
            = wldcore_map_begin_route_segment(&g_wldcore_map_travel, g_wldcore_map_travel.route_path.routes[0], 0);
        g_wldcore_map_travel.segment_index = 0;
        g_wldcore_map_projection_state.angle = g_wldcore_map_travel.heading;
        depth = g_wldcore_menu_stack_depth - 1;
        g_wldcore_map_travel.menu_level = depth;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth].window_index].flags
            |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        if (wldcore_map_start_projection_motion_if_outside_bounds(
                &g_wldcore_map_projection_state.marker.render.position, &g_wldcore_map_projection_state.marker.position)
            == 0) {
            g_wldcore_map_projection_state.flags
                |= WLDCORE_PROJECTION_FLAG_UPDATE_DIRECTION | WLDCORE_PROJECTION_FLAG_ALTERNATE_DIRECTION_SET;
        }
        g_wldcore_map_projection_state.marker.sub_kind = wldcore_gfx_load_location_picture(to_location);
        g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL;
        return 1;
    }
}
