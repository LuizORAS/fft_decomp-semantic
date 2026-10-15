#include "fft/wldcore.h"
#include "psx/libetc.h"
#include "psx/types.h"

/* Search path under construction; g_wldcore_route_search_depth is its length and g_wldcore_route_search_cost
 * its accumulated cost. */
extern wldcore_route_path_t g_wldcore_route_search_path;

/* Depth-first search for the best path between two locations over the visible routes
 * (EVENT_SCRIPT_VAR_PATH_VISIBLE_FIRST + route), at most 16 legs and never one route twice: a route
 * joining from and to completes a path, which is kept when it has fewer legs than the best so far, or
 * as many and no higher total cost (the routes' cost bytes); other routes leaving `from` recurse
 * from their far end. A long search keeps the world frame drawing: once more than 200 horizontal
 * blanks have passed since the last VSync (VSync(1)), a frame is drawn.
 *
 * `unused` keeps 16 frame bytes the target reserves and never touches. */
void wldcore_map_find_best_route_path(s32 from, s32 to) {
    s32 unused[4];
    s32 cost;
    s32 route;
    wldcore_route_record_t** table;
    s32 i;
    wldcore_route_record_t* record;
    s32 start;
    s32 end;
    s32 length;
    s32 buffer_index;

    if (g_wldcore_route_search_depth < 16) {
        table = g_wldcore_map_route_tables;
        for (route = 0; route < WLDCORE_ROUTE_COUNT; route++) {
            if (VSync(1) > 200) {
                main_file_poll_load(&g_main_file_cd_state);
                wldcore_sound_process_audio_queue();
                wldcore_map_pulse_dot_colors();
                wldcore_gfx_draw_world_frame();
                buffer_index = GsGetActiveBuff();
                g_main_gfx_active_buffer_index = buffer_index;
                GsSetWorkBase(&g_wldcore_gfx_world_primitive_buffers + buffer_index * 0xE000);
            }
            if (world_script_get_variable(route + EVENT_SCRIPT_VAR_PATH_VISIBLE_FIRST) == 0) {
                continue;
            }
            for (i = 0; i < g_wldcore_route_search_depth; i++) {
                if ((g_wldcore_route_search_path.routes[i] & 0xff) == route) {
                    break;
                }
            }
            if (i < g_wldcore_route_search_depth) {
                continue;
            }
            record = table[route];
            length = g_wldcore_route_search_depth + 1;
            start = record->start_location;
            end = record->end_location;
            cost = record->cost;
            if (g_wldcore_map_travel.best_length_bound < length) {
                continue;
            }
            if (length == g_wldcore_map_travel.best_length_bound
                && g_wldcore_map_travel.best_cost < cost + g_wldcore_route_search_cost) {
                continue;
            }
            if (from == start && to == end) {
                g_wldcore_route_search_path.routes[g_wldcore_route_search_depth] = route;
                /* Both directions record the path through one copy of this
                 * code (a second copy loop could not be cross-jumped away). */
                goto found;
            }
            if (to == start && from == end) {
                g_wldcore_route_search_path.routes[g_wldcore_route_search_depth] = route | 0x100;
            found:
                g_wldcore_map_travel.route_path = g_wldcore_route_search_path;
                g_wldcore_map_travel.best_cost = g_wldcore_route_search_cost + cost;
                g_wldcore_map_travel.best_length_bound = g_wldcore_map_travel.best_path_length
                    = g_wldcore_route_search_depth + 1;
                g_wldcore_map_travel.flags |= WLDCORE_ROUTE_TRAVEL_ACTIVE;
                continue;
            }
            if (from == start) {
                g_wldcore_route_search_path.routes[g_wldcore_route_search_depth] = route;
                g_wldcore_route_search_depth++;
                g_wldcore_route_search_cost += cost;
                wldcore_map_find_best_route_path(end, to);
                g_wldcore_route_search_depth--;
                g_wldcore_route_search_cost -= cost;
            } else if (from == end) {
                g_wldcore_route_search_path.routes[g_wldcore_route_search_depth] = route | 0x100;
                g_wldcore_route_search_depth++;
                g_wldcore_route_search_cost += cost;
                wldcore_map_find_best_route_path(start, to);
                g_wldcore_route_search_depth--;
                g_wldcore_route_search_cost -= cost;
            }
        }
    }
}
