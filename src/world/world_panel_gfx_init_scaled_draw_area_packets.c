#include "fft/world.h"
#include "psx/types.h"

/* The unit panels' copy of the draw-area pair reset (the BUNIT and EQUIP twin). */
void world_panel_gfx_init_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* pair) {
    pair->rects[0].w = 0;
    pair->rects[1].h = 0;
    world_util_copy_bytes(&pair->rects[1], &g_world_gfx_draw_area_template, 8);
}
