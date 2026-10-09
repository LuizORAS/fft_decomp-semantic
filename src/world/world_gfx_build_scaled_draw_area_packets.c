/* One `scaled` local is shared across both /200 divisions, as the target puts
 * both results in $t3. */
#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Callers pass different thread records; only the halfwords at offsets 0 and
 * 4 are read here. Copying the view into a local reorders the prologue's
 * saved-register moves. */
void world_gfx_build_scaled_draw_area_packets(
    world_gfx_scaled_draw_area_pair_t* packets, const void* geometry, s32 buffer, s32 frame_offset, void* thread) {
    s32 percent;
    s32 w;
    s32 h;
    s32 scaled;

    if (g_world_event_speed == 2) {
        buffer *= 2;
    }
    if (buffer >= 12) {
        buffer = 11;
    }
    percent = g_world_gfx_panel_draw_area_scale_percent[buffer];
    world_script_copy_bytes(&packets->rects[0], geometry, 8);
    w = packets->rects[0].w;
    h = packets->rects[0].h;
    packets->rects[0].x -= 0x80;
    packets->rects[0].x += ((const RECT*)thread)->x;
    packets->rects[0].y += ((const RECT*)thread)->w;
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
