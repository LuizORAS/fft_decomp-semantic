#include "fft/memory.h"
#include "fft/world.h"

/* Reset the first draw-area width and restore the second rectangle. */
void world_option_gfx_init_scaled_draw_area_packets(scaled_draw_area_pair_t* pair) {
    pair->rects[0].w = 0;
    pair->rects[1].h = 0;
    copy_bytes_nonempty(&pair->rects[1], &g_world_option_gfx_draw_area_template, sizeof(pair->rects[1]));
}
