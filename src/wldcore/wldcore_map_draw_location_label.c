#include "fft/wldcore.h"

/* Draw the name label of the location in g_wldcore_map_label_location (one-based, 0 for none) at
 * its dot: the label sequence WLDCORE_ANIM_SEQUENCE_LOCATION_NAME_FIRST + location, in the label
 * shade tinted by the screen fade. The map cursor sets the location it hovers over, and a script's
 * focus request the location it focuses; MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL hides it.
 *
 * sequence is written after the tint call because the target reloads the label location
 * there. */
void wldcore_map_draw_location_label(GsOT* ot) {
    world_animated_render_record_t request;
    CVECTOR color;
    s32 index;
    s32 shade;

    if (g_main_system_flags & MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL) {
        return;
    }
    if (g_wldcore_map_label_location == 0) {
        return;
    }

    index = g_wldcore_map_label_location - 1;
    request.flags = WLDCORE_DISPLAY_OBJECT_ANIMATED;
    request.position.x = g_wldcore_map_dots[index].render.position.x;
    request.position.y = g_wldcore_map_dots[index].render.position.y;
    shade = g_wldcore_map_label_shade;
    request.priority = 4;
    request.palette = 0;
    request.anim_counter = 0;
    request.frame_index = 0;
    color.r = shade;
    color.g = shade;
    color.b = shade;
    wldcore_gfx_copy_color_with_tint(&color, &color);
    request.color.r = color.r;
    request.color.g = color.g;
    request.color.b = color.b;
    request.sequence = g_wldcore_map_label_location + (WLDCORE_ANIM_SEQUENCE_LOCATION_NAME_FIRST - 1);
    wldcore_gfx_draw_animated_display_object(&request, ot);
}
