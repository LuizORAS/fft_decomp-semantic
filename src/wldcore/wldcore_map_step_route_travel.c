#include "fft/wldcore.h"

/* Move the travelling marker one frame along its path; the live map frame calls it. While a camera
 * pan runs it only advances the pan. Otherwise the marker moves between its segment's points, the
 * map scrolling to keep it in view, and the next segment starts when one ends. At the end of each leg
 * a day passes (wldcore_advance_calendar_day), the marker stands on the leg's location
 * (EVENT_SCRIPT_VAR_LOCATION), and that location is checked in order:
 *  - a story event there (WLDCORE_SCRIPT_ACTION_LOAD_EVENT) starts through the event transition;
 *  - its pending SetVar2 is applied;
 *  - a random battle on the road just taken (wldcore_map_roll_random_encounter) starts through the
 *    spinning transition, with MAIN_SYSTEM_FLAG_RANDOM_BATTLE, its map, ENTD and deployment squad;
 *  - otherwise the arrival sound plays and the next leg starts or, at the destination, the cursor and
 *    the label come back and a red location's event choices open its menu.
 * The arrival also writes the location id into the cursor level's record through the
 * location_transition view, whose location_id is the root level's one-based hovered_location (see
 * QUIRKS.md).
 *
 * The two event-push tails are written out in full: GCC 2.7.2 cross-jumps
 * them into the single shared tail at 0x8008e770, and only per-path CSE of
 * each copy keeps the target's direct g_wldcore_location_view accesses and the &state
 * base register in the flags update. */
void wldcore_map_step_route_travel(void) {
    wldcore_point32_t delta;
    SVECTOR previous;
    s32 dx;
    s32 dy;
    s32 result;

    if (!(g_wldcore_map_travel.flags & WLDCORE_ROUTE_TRAVEL_ACTIVE)) {
        return;
    }
    if (g_wldcore_map_projection_motion.flags & WLDCORE_PROJECTION_MOTION_ACTIVE) {
        if (wldcore_map_step_projection_motion() == 0) {
            g_wldcore_location_view.projection_state
                |= WLDCORE_PROJECTION_FLAG_UPDATE_DIRECTION | WLDCORE_PROJECTION_FLAG_ALTERNATE_DIRECTION_SET;
        }
        g_wldcore_location_view.projection_state |= WLDCORE_PROJECTION_FLAG_UPDATE_POSITION;
        return;
    }
    if (g_wldcore_map_travel.segment_frame >= g_wldcore_map_travel.segment_frames) {
        if (++g_wldcore_map_travel.segment_index >= g_wldcore_map_travel.segment_count) {
            wldcore_advance_calendar_day();
            g_wldcore_location_view.coordinates = g_wldcore_map_travel.target_position;
            g_wldcore_location_view.projection_state
                |= WLDCORE_PROJECTION_FLAG_UPDATE_POSITION | WLDCORE_PROJECTION_FLAG_UPDATE_DIRECTION;
            g_wldcore_location_view.location_id = g_wldcore_map_travel.leg_end_location;
            world_script_set_variable(EVENT_SCRIPT_VAR_LOCATION, g_wldcore_map_travel.leg_end_location);
            g_wldcore_map_travel.route_index++;
            if (wldcore_script_process_conditional_set(
                    g_wldcore_location_view.location_id, WLDCORE_SCRIPT_ACTION_LOAD_EVENT)
                != 0) {
                g_wldcore_next_map_id[0] = 0;
                world_script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, g_wldcore_script_state.args[0]);
                wldcore_menu_push_event_transition_level(g_wldcore_script_state.args[1]);
                g_wldcore_location_view.angle = 0;
                g_wldcore_location_view.projection_state &= ~WLDCORE_PROJECTION_FLAG_ALTERNATE_DIRECTION_SET;
                g_wldcore_map_travel.flags &= ~(WLDCORE_ROUTE_TRAVEL_ACTIVE | WLDCORE_ROUTE_TRAVEL_REVERSED);
                g_wldcore_menu_stack_records_next[g_wldcore_map_travel.menu_level].location_transition.location_id
                    = g_wldcore_location_view.location_id;
                return;
            }
            if (wldcore_script_process_conditional_set(
                    g_wldcore_location_view.location_id, WLDCORE_SCRIPT_ACTION_SET_VAR_2)
                != 0) {
                world_script_set_variable(g_wldcore_script_state.args[0], g_wldcore_script_state.args[1]);
            }
            if (wldcore_map_roll_random_encounter(g_wldcore_map_travel.leg_end_location,
                    g_wldcore_map_travel.route_path.routes[g_wldcore_map_travel.route_index - 1] & 0xFF)
                != 0) {
                g_main_system_flags |= MAIN_SYSTEM_FLAG_RANDOM_BATTLE;
                g_wldcore_next_map_id[0] = g_wldcore_random_battle_map_id;
                world_script_set_variable(EVENT_SCRIPT_VAR_CURRENT_ENTD, g_wldcore_random_battle_entd_id);
                world_script_set_variable(EVENT_SCRIPT_VAR_DEPLOYMENT_SQUAD_COUNT, 1);
                world_script_set_variable(
                    EVENT_SCRIPT_VAR_DEPLOYMENT_PRIMARY_SQUAD_ID, g_wldcore_random_battle_squad_id);
                world_script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, 0);
                wldcore_menu_push_event_transition_level(WLDCORE_EVENT_TRANSITION_MODE_SPIN);
                g_wldcore_location_view.angle = 0;
                g_wldcore_location_view.projection_state &= ~WLDCORE_PROJECTION_FLAG_ALTERNATE_DIRECTION_SET;
                g_wldcore_map_travel.flags &= ~(WLDCORE_ROUTE_TRAVEL_ACTIVE | WLDCORE_ROUTE_TRAVEL_REVERSED);
                g_wldcore_menu_stack_records_next[g_wldcore_map_travel.menu_level].location_transition.location_id
                    = g_wldcore_location_view.location_id;
                return;
            }
            wldcore_sound_play_effect(MAIN_SFX_WORLD_MAP_ARRIVE);
            if (g_wldcore_map_travel.route_index < g_wldcore_map_travel.best_path_length) {
                g_wldcore_map_travel.flags &= ~WLDCORE_ROUTE_TRAVEL_REVERSED;
                g_wldcore_map_travel.segment_count = wldcore_map_begin_route_segment(
                    &g_wldcore_map_travel, g_wldcore_map_travel.route_path.routes[g_wldcore_map_travel.route_index], 0);
                g_wldcore_map_travel.segment_index = 0;
                g_wldcore_location_view.angle = g_wldcore_map_travel.heading;
                return;
            }
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_map_travel.menu_level]
                                         .location_transition.window_index]
                .flags ^= WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_map_travel.menu_level]
                                         .location_transition.window_index]
                .position.x = g_wldcore_location_view.point.x;
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_map_travel.menu_level]
                                         .location_transition.window_index]
                .position.y = g_wldcore_location_view.point.y - 4;
            g_main_system_flags &= ~MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL;
            g_wldcore_map_travel.arrival_location = g_wldcore_location_view.location_id + 1;
            wldcore_wait_for_file_load();
            g_wldcore_location_view.angle = 0;
            g_wldcore_location_view.projection_state &= ~WLDCORE_PROJECTION_FLAG_ALTERNATE_DIRECTION_SET;
            g_wldcore_map_travel.flags &= ~(WLDCORE_ROUTE_TRAVEL_ACTIVE | WLDCORE_ROUTE_TRAVEL_REVERSED);
            g_wldcore_menu_stack_records_next[g_wldcore_map_travel.menu_level].location_transition.location_id
                = g_wldcore_location_view.location_id;
            result = wldcore_location_collect_choice_text_ids(g_wldcore_location_view.location_id,
                (s32*)g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
                    .location_transition.transition_data);
            if (result != -1 && result > 0) {
                g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].location_transition.pending_result
                    = result;
                wldcore_menu_dispatch_publish_handler();
                wldcore_menu_push_location_menu_level(g_wldcore_location_view.location_id);
            }
            return;
        } else {
            wldcore_map_begin_route_segment(
                &g_wldcore_map_travel, g_wldcore_map_travel.route_id, g_wldcore_map_travel.segment_index);
            g_wldcore_location_view.angle = g_wldcore_map_travel.heading;
            g_wldcore_location_view.projection_state |= WLDCORE_PROJECTION_FLAG_UPDATE_DIRECTION;
        }
    } else {
        g_wldcore_map_travel.segment_frame++;
    }
    dx = (g_wldcore_map_travel.target_position.vx - g_wldcore_map_travel.position.vx)
        * g_wldcore_map_travel.segment_frame / g_wldcore_map_travel.segment_frames;
    dy = (g_wldcore_map_travel.target_position.vy - g_wldcore_map_travel.position.vy)
        * g_wldcore_map_travel.segment_frame / g_wldcore_map_travel.segment_frames;
    previous = g_wldcore_location_view.coordinates;
    delta.x = g_wldcore_map_travel.previous_position.vx + dx;
    g_wldcore_location_view.coordinates.vx = delta.x;
    delta.y = g_wldcore_map_travel.previous_position.vy + dy;
    g_wldcore_location_view.coordinates.vy = delta.y;
    delta.x -= previous.vx;
    delta.y -= previous.vy;
    wldcore_map_scroll_projection_if_focus_near_edge(&delta);
    g_wldcore_location_view.projection_state |= WLDCORE_PROJECTION_FLAG_UPDATE_POSITION;
}
