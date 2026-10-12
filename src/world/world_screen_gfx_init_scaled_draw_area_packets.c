#include "fft/world.h"

/* The menu-screen library's copy of the draw-area pair reset; nothing calls it. */
void world_screen_gfx_init_scaled_draw_area_packets(scaled_draw_area_pair_t* pair) {
    pair->rects[0].w = 0;
    pair->rects[1].h = 0;
    world_script_copy_bytes(&pair->rects[1], &g_world_gfx_full_texture_window, sizeof(pair->rects[1]));
}
