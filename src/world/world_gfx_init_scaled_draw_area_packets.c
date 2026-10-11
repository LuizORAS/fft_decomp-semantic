#include "fft/world.h"
#include "psx/types.h"

/* Clear the first draw area's width and install the default second draw area. */
void world_gfx_init_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* pair) {
    pair->rects[0].w = 0;
    pair->rects[1].h = 0;
    world_script_copy_bytes(&pair->rects[1], &g_world_gfx_texture_window, sizeof(pair->rects[1]));
}
