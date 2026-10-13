#include "fft/battle.h"
#include "fft/battle_gfx.h"
#include "fft/battle_menu.h"
#include "fft/memory.h"

/* Clear the first draw area's width and install the default second draw area. */
void battle_gfx_init_scaled_draw_area_packets(scaled_draw_area_pair_t* pair) {
    pair->rects[0].w = 0;
    pair->rects[1].h = 0;
    copy_bytes_nonempty(&pair->rects[1], &g_battle_menu_disabled_texture_window, 8);
}
