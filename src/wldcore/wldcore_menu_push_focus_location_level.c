#include "fft/wldcore.h"
#include "psx/types.h"

/* Push the focus location level (WLDCORE_MENU_LEVEL_FOCUS_LOCATION) for a dot: its record keeps the
 * dot index, and when the dot calls for a camera pan one starts, hiding the location label and the
 * parent's window while it runs. The map cursor pushes it for the current location when Circle
 * finds no dot, and a location's script for the location it focuses.
 *
 * Needs aspsx 2.21 (the module default); 2.34 emits the three-instruction $at
 * expansion for the indexed global stores and is 6 instructions short.
 *
 * The two call arguments carry the whole shape. The target materialises
 * g_wldcore_map_dots + 0x10 once, derives the motion point as that pointer + 0x18 and
 * the coordinate pair as the same constant - 8 (cse's related-value form).
 * Holding a single `wldcore_map_dot_t* dot` instead lets plus_constant fold
 * both displacements into the symbol, which costs the `addiu a0,a0,24`.
 * Anchoring the first argument on the +0x10 sub-record and spelling the second
 * as a direct array index restores both. */
void wldcore_menu_push_focus_location_level(s32 dot_index) {
    world_animated_render_record_t* render;

    render = &g_wldcore_map_dots[dot_index].render;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].focus_dot_index = dot_index;
    wldcore_map_start_projection_motion_if_outside_bounds(&render->position, &g_wldcore_map_dots[dot_index].position);
    if (g_wldcore_map_projection_motion.flags & WLDCORE_PROJECTION_MOTION_ACTIVE) {
        g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1].window_index].flags
            |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    }
    g_wldcore_menu_stack_types[g_wldcore_menu_stack_depth + 1] = WLDCORE_MENU_LEVEL_FOCUS_LOCATION;
    g_wldcore_menu_stack_depth = g_wldcore_menu_stack_depth + 1;
}
