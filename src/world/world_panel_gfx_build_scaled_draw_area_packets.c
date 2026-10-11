/* One `scaled` local is shared across both /200 divisions, as the target puts
 * both results in $t3. */
#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* The unit panels' copy of the scaled draw-area builder (the BUNIT and EQUIP twin).
 * The panel origin is narrowed to the packet's 16-bit coordinates. */
void world_panel_gfx_build_scaled_draw_area_packets(scaled_draw_area_pair_t* packets, const RECT* geometry,
    s32 scale_index, s32 frame_offset, const menu_status_thread_t* thread) {
    s32 percent;
    s32 w;
    s32 h;
    s32 scaled;

    if (g_world_event_speed == 2) {
        scale_index *= 2;
    }
    if (scale_index >= 12) {
        scale_index = 11;
    }
    percent = g_world_gfx_panel_draw_area_scale_percent[scale_index];
    world_script_copy_bytes(&packets->rects[0], geometry, sizeof(*geometry));
    w = packets->rects[0].w;
    h = packets->rects[0].h;
    packets->rects[0].x -= 0x80;
    packets->rects[0].x += thread->x;
    packets->rects[0].y += thread->y;
    scaled = (w * percent) / 200;
    packets->rects[0].x = w / 2 + packets->rects[0].x - scaled;
    scaled = (h * percent) / 200;
    packets->rects[0].y = h / 2 + packets->rects[0].y - scaled;
    packets->rects[0].w = (packets->rects[0].w * percent) / 100;
    packets->rects[0].h = (packets->rects[0].h * percent) / 100;
    if (frame_offset != 0) {
        packets->rects[1].y = 0;
    } else {
        packets->rects[1].y = 0xF0;
        packets->rects[0].y += 0xF0;
    }
    SetDrawArea(&packets->areas[0], &packets->rects[0]);
    SetDrawArea(&packets->areas[1], &packets->rects[1]);
}
