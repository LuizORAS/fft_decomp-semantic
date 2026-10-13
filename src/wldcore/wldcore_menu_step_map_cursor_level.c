#include "fft/wldcore.h"
#include "psx/libgte.h"
#include "psx/pad.h"

/* The map camera block at 0x8009f2c0. The target relates the
 * origin and scale addresses to one symbol (origin.vx is kept in a register
 * while both scale stores use the absolute form), so they are one object. */
typedef struct wldcore_map_projection_view {
    VECTOR origin;
    VECTOR scale;
} wldcore_map_projection_view_t;

extern wldcore_map_projection_view_t g_wldcore_map_projection_view;

/* View of wldcore_window_record_t with its x/y pair as one
 * point: the target passes records[i].x/y by value with two absolute-form
 * loads, which only a real point member reproduces (a local copy spills to
 * the stack and a cast view computes the address into a register). */
typedef struct wldcore_window_position_record {
    u8 _unused_00[0x18];
    wldcore_point32_t position;
    s32 _unused_20;
} wldcore_window_position_record_t;
extern wldcore_window_position_record_t g_wldcore_window_position_records[];

/* Step of the root level, the world map cursor. While the marker travels it only keeps the map
 * drawing. Otherwise, when no fade, zoom or audio command is pending:
 *  - Circle on another location's dot travels there (wldcore_map_start_travel), on the current
 *    location opens its menu when it offers entries, and off any dot pans the camera back to the
 *    current location (the focus location level).
 *  - Triangle or Start opens WORLD's menu (wldcore_menu_push_world_menu_level).
 *  - Select on a dot shows that location's help text over the captured map.
 *  - L1 or R1 zooms out to the whole map (scale 0.5, the dots and routes hidden) or back in, over
 *    32 frames.
 *  - Holding Square hides the cursor behind the edge arrows, and the D-pad scrolls the map 4 pixels
 *    a frame (free_scroll).
 *  - Otherwise the D-pad moves the cursor with acceleration, pulls it toward the dot it overlaps
 *    (wldcore_map_get_dot_snap_step), scrolls the map when the cursor nears the screen edge and keeps
 *    it within x -0x70..0x78, y -0x68..0x68.
 * The location under the cursor becomes the label location (g_wldcore_map_label_location).
 * pending_mode would unblock the input after a fade (3 also ends the visit), but only the push writes
 * it, with 0.
 *
 * The zoom interpolation uses four signed divisions by `duration`
 * (gcc-2.7.2_O2_aspsx-2.21_divcheck). `point` is never written; it
 * reproduces the target's unused 8-byte slot at sp+0x18. The chained scale
 * stores keep each branch's scale value a single-set temporary; one variable
 * shared by both branches is multi-set, is scheduled early and changes the
 * register allocation of the interpolation. */
void wldcore_menu_step_map_cursor_level(wldcore_menu_map_cursor_level_t* level) {
    wldcore_point32_t amount;
    wldcore_point32_t point;
    wldcore_point32_t target;
    VECTOR saved;
    s32 step_x;
    s32 step_y;
    s32 flags;
    s32 motion;
    s32 buttons;
    s32 held;
    s32 hit;
    s32 frame;
    s32 duration;

    /* The exits funnel into the shared snap-hit tail at `finish`; in the
     * target's block order these need gotos. */
    if (g_wldcore_map_travel.flags & WLDCORE_ROUTE_TRAVEL_ACTIVE) {
        g_main_system_flags |= MAIN_SYSTEM_FLAG_DRAW_MAP;
        goto finish;
    }

    if (level->free_scroll == 0 && !(g_wldcore_previous_system_flags & MAIN_SYSTEM_FLAG_DRAW_MAP)) {
        if (level->pending_mode != 0) {
            flags = g_main_system_flags;
            if (flags & MAIN_SYSTEM_FLAG_FADING) {
                return;
            }
            g_main_system_flags = flags ^ MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
            if (level->pending_mode == 3) {
                g_main_system_flags = flags ^ (MAIN_SYSTEM_FLAG_RUNNING | MAIN_SYSTEM_FLAG_INPUT_BLOCKED);
            }
            level->pending_mode = 0;
        }
        if (g_main_system_flags & MAIN_SYSTEM_FLAG_FADING) {
            /* Skips the zoom handling and the wide-view toggle below. */
            goto scroll;
        }
        if (!(g_wldcore_map_zoom_motion.flags
                & (WLDCORE_PROJECTION_MOTION_ACTIVE | WLDCORE_PROJECTION_MOTION_ZOOMED_OUT))
            && g_wldcore_audio_queue.count == 0) {
            if (g_wldcore_new_button_presses & PSX_PAD_CIRCLE) {
                hit = wldcore_map_find_dot_at_point(g_wldcore_window_position_records[level->window_index].position);
                if (hit != 0) {
                    wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
                    hit--;
                    if (hit != g_wldcore_map_projection_state.marker.kind) {
                        wldcore_map_start_travel(g_wldcore_map_projection_state.marker.kind, hit);
                        return;
                    }
                    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].panel.entry_count
                        = wldcore_map_build_location_menu_entries(
                            hit, g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].panel.entries);
                    if (g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].panel.entry_count != 0) {
                        wldcore_menu_suspend_root_level((wldcore_menu_stack_record_t*)level);
                        wldcore_menu_push_location_menu_level(hit);
                        return;
                    }
                } else {
                    g_wldcore_window_records[level->window_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
                    wldcore_menu_push_focus_location_level(g_wldcore_map_projection_state.marker.kind);
                    return;
                }
            }
            buttons = g_wldcore_new_button_presses;
            if (buttons & (PSX_PAD_TRIANGLE | PSX_PAD_START)) {
                wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
                wldcore_menu_suspend_root_level((wldcore_menu_stack_record_t*)level);
                wldcore_menu_push_world_menu_level();
                return;
            }
            if (buttons & PSX_PAD_SELECT) {
                hit = wldcore_map_find_dot_at_point(g_wldcore_window_position_records[level->window_index].position);
                if (hit != 0) {
                    hit--;
                    wldcore_menu_suspend_root_level((wldcore_menu_stack_record_t*)level);
                    wldcore_gfx_toggle_captured_world_frame(1);
                    wldcore_menu_push_message_level(hit + 0x8000, 1);
                    return;
                }
            }
        }
    }

    motion = g_wldcore_map_zoom_motion.flags;
    if (!(motion & WLDCORE_PROJECTION_MOTION_ACTIVE)) {
        if (level->free_scroll == 0 && (g_wldcore_new_button_presses & (PSX_PAD_L1 | PSX_PAD_R1))) {
            g_wldcore_window_records[level->window_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_map_zoom_motion.progress = 0x20;
            g_wldcore_map_zoom_motion.distance = 0;
            g_wldcore_map_zoom_motion.flags = (g_wldcore_map_zoom_motion.flags ^ WLDCORE_PROJECTION_MOTION_ZOOMED_OUT)
                | WLDCORE_PROJECTION_MOTION_ACTIVE;
            if (g_wldcore_map_zoom_motion.flags & WLDCORE_PROJECTION_MOTION_ZOOMED_OUT) {
                g_wldcore_map_zoom_motion.origin = g_wldcore_map_projection_view.origin;
            }
            level->hovered_location = 0;
            if (g_wldcore_map_zoom_motion.flags & WLDCORE_PROJECTION_MOTION_ZOOMED_OUT) {
                wldcore_sound_play_effect(MAIN_SFX_WORLD_MAP_ZOOM_OUT);
            } else {
                wldcore_sound_play_effect(MAIN_SFX_WORLD_MAP_ZOOM_IN);
            }
            goto finish;
        }
    } else {
        frame = g_wldcore_map_zoom_motion.distance;
        duration = g_wldcore_map_zoom_motion.progress;
        step_x = (frame << 11) / duration;
        if (motion & WLDCORE_PROJECTION_MOTION_ZOOMED_OUT) {
            g_wldcore_map_projection_view.origin.vx
                = g_wldcore_map_zoom_motion.origin.vx + ((0 - g_wldcore_map_zoom_motion.origin.vx) * frame) / duration;
            g_wldcore_map_projection_view.origin.vy
                = g_wldcore_map_zoom_motion.origin.vy + ((8 - g_wldcore_map_zoom_motion.origin.vy) * frame) / duration;
            g_wldcore_map_projection_view.scale.vx = g_wldcore_map_projection_view.scale.vy = ONE - step_x;
            g_wldcore_map_zoom_motion.distance = frame + 1;
            g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_MAP_DOTS | MAIN_SYSTEM_FLAG_HIDE_MAP_ROUTES;
            if (frame + 1 == duration) {
                g_wldcore_map_projection_view.scale.vy = 0x800;
                g_wldcore_map_projection_view.scale.vx = 0x800;
                g_wldcore_map_projection_view.origin.vx = 0;
                g_wldcore_map_projection_view.origin.vy = 8;
                g_wldcore_map_zoom_motion.flags = motion ^ WLDCORE_PROJECTION_MOTION_ACTIVE;
            }
        } else {
            g_wldcore_map_projection_view.origin.vx = (g_wldcore_map_zoom_motion.origin.vx * frame) / duration;
            g_wldcore_map_projection_view.origin.vy
                = ((g_wldcore_map_zoom_motion.origin.vy - 8) * frame) / duration + 8;
            g_wldcore_map_projection_view.scale.vx = g_wldcore_map_projection_view.scale.vy = step_x + 0x800;
            g_wldcore_map_zoom_motion.distance = frame + 1;
            if (frame + 1 == duration) {
                g_wldcore_map_projection_view.scale.vy = ONE;
                g_wldcore_map_projection_view.scale.vx = ONE;
                g_wldcore_map_projection_view.origin = g_wldcore_map_zoom_motion.origin;
                g_wldcore_map_zoom_motion.flags ^= WLDCORE_PROJECTION_MOTION_ACTIVE;
                g_main_system_flags &= ~(MAIN_SYSTEM_FLAG_HIDE_MAP_DOTS | MAIN_SYSTEM_FLAG_HIDE_MAP_ROUTES);
            }
        }
        g_main_system_flags |= MAIN_SYSTEM_FLAG_DRAW_MAP;
        g_wldcore_map_projection_state.flags |= WLDCORE_PROJECTION_FLAG_UPDATE_POSITION;
        goto finish;
    }

    if (g_wldcore_map_zoom_motion.flags & WLDCORE_PROJECTION_MOTION_ZOOMED_OUT) {
        goto finish;
    }

    if (!(g_wldcore_current_button_input & PSX_PAD_SQUARE)) {
        if (level->free_scroll != 0) {
            level->free_scroll = 0;
            g_wldcore_window_records[level->arrows_window_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        }
        g_wldcore_window_records[level->window_index].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    } else if (level->free_scroll != 1) {
        level->free_scroll = 1;
        g_wldcore_window_records[level->window_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[level->arrows_window_index].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[level->arrows_window_index].anim_counter
            = g_wldcore_window_records[level->arrows_window_index].frame_index = 0;
        g_wldcore_map_cursor_scroll_y.value = 0;
        g_wldcore_map_cursor_scroll_x.value = 0;
        g_wldcore_map_cursor_scroll_y.output = 0;
        g_wldcore_map_cursor_scroll_x.output = 0;
    }

scroll:
    if (level->free_scroll == 0) {
        held = g_wldcore_current_button_input;
        if (held & (PSX_PAD_RIGHT | PSX_PAD_LEFT)) {
            if (held & PSX_PAD_LEFT) {
                wldcore_map_update_scroll_axis_speed(&g_wldcore_map_cursor_scroll_x, -1);
            } else {
                wldcore_map_update_scroll_axis_speed(&g_wldcore_map_cursor_scroll_x, 1);
            }
        } else {
            wldcore_map_update_scroll_axis_speed(&g_wldcore_map_cursor_scroll_x, 0);
        }
        held = g_wldcore_current_button_input;
        if (held & (PSX_PAD_UP | PSX_PAD_DOWN)) {
            if (held & PSX_PAD_UP) {
                wldcore_map_update_scroll_axis_speed(&g_wldcore_map_cursor_scroll_y, -1);
            } else {
                wldcore_map_update_scroll_axis_speed(&g_wldcore_map_cursor_scroll_y, 1);
            }
        } else {
            wldcore_map_update_scroll_axis_speed(&g_wldcore_map_cursor_scroll_y, 0);
        }
        amount.x = g_wldcore_map_cursor_scroll_x.output;
        amount.y = g_wldcore_map_cursor_scroll_y.output;
        if (!(g_wldcore_map_zoom_motion.flags & WLDCORE_PROJECTION_MOTION_ZOOMED_OUT)) {
            target.x = g_wldcore_window_records[level->window_index].position.x + amount.x;
            target.y = g_wldcore_window_records[level->window_index].position.y + amount.y;
            level->hovered_location = wldcore_map_get_dot_snap_step(target, &step_x, &step_y);
            amount.x += step_x;
            amount.y += step_y;
            target.x = g_wldcore_window_records[level->window_index].position.x + amount.x;
            target.y = g_wldcore_window_records[level->window_index].position.y + amount.y;
            saved = g_wldcore_map_projection_view.origin;
            if ((amount.x < 0 && target.x < -0x40) || (amount.x > 0 && target.x >= 0x49)) {
                wldcore_map_apply_clamped_horizontal_scroll(&amount.x, &g_wldcore_map_projection_view.origin.vx);
            }
            if ((amount.y < 0 && target.y < -0x40) || (amount.y > 0 && target.y >= 0x41)) {
                wldcore_map_apply_clamped_vertical_scroll(
                    &amount, (wldcore_point32_t*)&g_wldcore_map_projection_view.origin);
            }
            if (saved.vx != g_wldcore_map_projection_view.origin.vx
                || saved.vy != g_wldcore_map_projection_view.origin.vy) {
                g_main_system_flags |= MAIN_SYSTEM_FLAG_DRAW_MAP;
                g_wldcore_map_projection_state.flags |= WLDCORE_PROJECTION_FLAG_UPDATE_POSITION;
            }
        }
        g_wldcore_window_records[level->window_index].position.x += amount.x;
        g_wldcore_window_records[level->window_index].position.y += amount.y;
        if (g_wldcore_window_records[level->window_index].position.x < -0x70) {
            g_wldcore_window_records[level->window_index].position.x = -0x70;
        }
        if (g_wldcore_window_records[level->window_index].position.x >= 0x79) {
            g_wldcore_window_records[level->window_index].position.x = 0x78;
        }
        if (g_wldcore_window_records[level->window_index].position.y < -0x68) {
            g_wldcore_window_records[level->window_index].position.y = -0x68;
        }
        if (g_wldcore_window_records[level->window_index].position.y >= 0x69) {
            g_wldcore_window_records[level->window_index].position.y = 0x68;
        }
    } else {
        held = g_wldcore_current_button_input;
        if (held & PSX_PAD_DPAD_MASK) {
            amount.y = 0;
            amount.x = 0;
            if (held & PSX_PAD_LEFT) {
                amount.x = -4;
            }
            if (held & PSX_PAD_RIGHT) {
                amount.x = 4;
            }
            if (held & PSX_PAD_UP) {
                amount.y = -4;
            }
            if (held & PSX_PAD_DOWN) {
                amount.y = 4;
            }
            saved = g_wldcore_map_projection_view.origin;
            wldcore_map_apply_clamped_scroll(&amount, (wldcore_point32_t*)&g_wldcore_map_projection_view.origin);
            if (saved.vx != g_wldcore_map_projection_view.origin.vx
                || saved.vy != g_wldcore_map_projection_view.origin.vy) {
                g_main_system_flags |= MAIN_SYSTEM_FLAG_DRAW_MAP;
                g_wldcore_map_projection_state.flags |= WLDCORE_PROJECTION_FLAG_UPDATE_POSITION;
            }
        }
    }

finish:
    if (level->free_scroll == 0 && (hit = level->hovered_location) != 0) {
        g_wldcore_map_label_location = hit;
        return;
    }
    g_wldcore_map_label_location = 0;
}
