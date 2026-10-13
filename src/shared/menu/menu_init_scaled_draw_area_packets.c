#include "fft/battle.h"
#include "fft/menu.h"

/* Clear the first draw area's width and install the default second draw area. */
void menu_init_scaled_draw_area_packets(scaled_draw_area_pair_t* data) {
    data->rects[0].w = 0;
    data->rects[1].h = 0;
    battle_copy_bytes(&data->rects[1], &g_menu_draw_area_template, 8);
}
