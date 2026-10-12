#include "fft/event_equip.h"

/* Clear the first draw area's width and install the default second draw area. */
void equip_gfx_init_scaled_draw_area_packets(scaled_draw_area_pair_t* data) {
    data->rects[0].w = 0;
    data->rects[1].h = 0;
    battle_copy_bytes(&data->rects[1], &g_equip_gfx_draw_area_template, 8);
}
