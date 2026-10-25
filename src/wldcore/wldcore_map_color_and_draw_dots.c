#include "fft/script.h"
#include "fft/wldcore.h"
#include "psx/types.h"

/* Build the 43 location dots and queue them for drawing. Each takes its kind, sub-kind and map
 * position from g_wldcore_map_dot_sources; kinds 0 and 1 (the castles and towns) use the first dot
 * sequence and kind 2 (the other places) the second. A location whose color variable
 * (EVENT_SCRIPT_VAR_LOCATION_COLOR_FIRST + id) is set takes palette 0xc (the first 24 locations) or
 * 0x10 (the fields), the blue and green dots; 0 keeps the sprite's own red, where a story event
 * waits. A location whose visibility variable is 0 is hidden. Resets the pulse and projects the
 * dots. */
void wldcore_map_color_and_draw_dots(void) {
    wldcore_map_dot_source_t* src;
    wldcore_map_dot_t* dots;
    wldcore_map_dot_t* dot;
    s32* flags_base;
    CVECTOR* color;
    s32 y;
    s32 is_site;
    s32 queue_index;
    s32 i;

    dots = g_wldcore_map_dots;
    /* Queue entries are built from the flags column base; &dots[i].render.flags does not match. */
    flags_base = &dots->render.flags;
    dot = dots;
    src = g_wldcore_map_dot_sources;
    g_wldcore_map_dot_pulse_direction = 0;
    g_wldcore_map_dot_pulse_phase = 0;
    g_wldcore_map_dot_count = 0;
    for (i = 0; i < WORLD_LOCATION_COUNT; i++) {
        g_wldcore_map_dots[i].kind = (u8)(src[i].position_and_kind >> 16);
        g_wldcore_map_dots[i].sub_kind = (u8)(src[i].position_and_kind >> 24);
        /* The x coordinate occupies the packed 32-bit value's low 16 bits. */
        g_wldcore_map_dots[i].position.vx = src[i].position_and_kind;
        y = src[i].y;
        g_wldcore_map_dots[i].render.flags = WLDCORE_DISPLAY_OBJECT_ANIMATED;
        g_wldcore_map_dots[i].render.priority = 0xf;
        /* Direct nested access biases the base to render; retail addresses the color column. */
        color = (CVECTOR*)((u8*)&g_wldcore_map_dots[i] + offsetof(wldcore_map_dot_t, render.color));
        g_wldcore_map_dots[i].position.vz = 0;
        g_wldcore_map_dots[i].position.vy = y;
        color->r = 0x80;
        color->g = 0x80;
        color->b = 0x80;
        is_site = g_wldcore_map_dots[i].kind == 2;
        g_wldcore_map_dots[i].render.sequence = is_site + WLDCORE_ANIM_SEQUENCE_MAP_DOT;
        if (script_get_variable(i + EVENT_SCRIPT_VAR_LOCATION_COLOR_FIRST) != 0) {
            if (i < WORLD_LOCATION_FIELD_FIRST) {
                g_wldcore_map_dots[i].render.palette = 0xc;
            } else {
                g_wldcore_map_dots[i].render.palette = 0x10;
            }
        } else {
            g_wldcore_map_dots[i].render.palette = 0;
        }
        dot->render.anim_counter = 0;
        g_wldcore_map_dots[i].render.frame_index = 0;
        if (script_get_variable(i + EVENT_SCRIPT_VAR_LOCATION_VISIBLE_FIRST) == 0) {
            g_wldcore_map_dots[i].render.flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        }
        dot++;
        queue_index = g_wldcore_window_render_object_count;
        g_wldcore_window_render_object_queue[queue_index] = (void*)(i * sizeof(wldcore_map_dot_t) + (s32)flags_base);
        g_wldcore_window_render_object_count = queue_index + 1;
    }
    g_wldcore_map_dot_count = WORLD_LOCATION_COUNT;
    wldcore_map_project_and_cull_dots();
}
