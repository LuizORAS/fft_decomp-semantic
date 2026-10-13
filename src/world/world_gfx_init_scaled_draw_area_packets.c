#include "fft/memory.h"
#include "fft/world.h"

/* Clear the first draw area's width and install the default second draw area. */
void world_gfx_init_scaled_draw_area_packets(scaled_draw_area_pair_t* pair) {
    pair->rects[0].w = 0;
    pair->rects[1].h = 0;
    mem_copy_bytes_nonempty(&pair->rects[1], &g_world_gfx_texture_window, sizeof(pair->rects[1]));
}
