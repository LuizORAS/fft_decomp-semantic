#include "fft/world.h"
#include "psx/types.h"

/* The menu-screen library's copy of the draw-area pair reset; nothing calls it. */
void world_screen_gfx_init_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* pair) {
    pair->rects[0].w = 0;
    pair->rects[1].h = 0;
    world_util_copy_bytes(&pair->rects[1], &g_world_gfx_full_texture_window, 8);
}
