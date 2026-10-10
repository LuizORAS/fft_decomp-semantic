#include "fft/event_bunit.h"
#include "psx/types.h"

/* Clear the first draw area's width and install the default second draw area. */
void bunit_gfx_init_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* packet) {
    packet->rects[0].w = 0;
    packet->rects[1].h = 0;
    battle_copy_bytes(&packet->rects[1], &g_bunit_gfx_draw_area_template, 8);
}
