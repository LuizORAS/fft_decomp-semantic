#include "fft/wldcore.h"
#include "psx/types.h"

void wldcore_gfx_draw_calendar_date(GsOT* ot) {
    wldcore_window_record_t request;
    CVECTOR color;
    s32 first;
    s32 second;

    if (g_main_system_flags & 0x1000) {
        return;
    }

    request.flags = 1;
    request.priority = 4;
    request.palette = 0;
    request.anim_counter = 0;
    request.frame_index = 0;
    request.position.x = g_wldcore_date_display_x;
    request.position.y = g_wldcore_date_display_y;
    wldcore_gfx_copy_color_with_tint(&g_wldcore_hud_color, &color);
    request.color.r = color.r;
    request.color.g = color.g;
    request.color.b = color.b;

    first = world_script_get_variable(EVENT_SCRIPT_VAR_MONTH);
    second = world_script_get_variable(EVENT_SCRIPT_VAR_DAY);
    request.sequence = first + 0x1F;
    wldcore_gfx_draw_animated_display_object(&request, ot);

    request.position.x += 0x1A;
    request.position.y -= 4;
    if (second >= 10) {
        request.anim_counter = 0;
        request.frame_index = 0;
        request.sequence = second / 10 + 0x2C;
        wldcore_gfx_draw_animated_display_object(&request, ot);
        request.position.x += 8;
    }
    request.sequence = second % 10 + 0x2C;
    wldcore_gfx_draw_animated_display_object(&request, ot);
}
