#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/*
 * Build a double-buffered draw-area pair whose rectangle is cropped to a
 * percentage of `geometry` about its centre. Sources at x < 0x80 are moved
 * to the right-hand page (x + 0x80, y + 0x78); others are moved left.
 */
void world_gfx_build_scaled_draw_area_pair_swapped(
    scaled_draw_area_pair_t* packets, const RECT* geometry, s32 step, s32 flag) {
    s32 percent;
    s16 w;
    s16 h;
    s32 half_extent;

    if (g_world_event_speed == 2) {
        step *= 2;
    }
    if (step >= 12) {
        step = 11;
    }
    percent = g_world_menu_cursor_zoom_percentages[step];
    world_script_copy_bytes(&packets->rects[0], geometry, sizeof(*geometry));
    if (packets->rects[0].x < 0x80) {
        packets->rects[0].x += 0x80;
        packets->rects[0].y += 0x78;
    } else {
        packets->rects[0].x -= 0x80;
    }
    w = packets->rects[0].w;
    h = packets->rects[0].h;
    half_extent = (w * percent) / 200;
    packets->rects[0].x = w / 2 + packets->rects[0].x - half_extent;
    half_extent = (h * percent) / 200;
    packets->rects[0].y = h / 2 + packets->rects[0].y - half_extent;
    packets->rects[0].w = (packets->rects[0].w * percent) / 100;
    packets->rects[0].h = (packets->rects[0].h * percent) / 100;
    if (flag != 0) {
        packets->rects[1].y = 0;
    } else {
        packets->rects[1].y = 0xF0;
        packets->rects[0].y += 0xF0;
    }
    SetDrawArea(&packets->areas[0], &packets->rects[0]);
    SetDrawArea(&packets->areas[1], &packets->rects[1]);
}
