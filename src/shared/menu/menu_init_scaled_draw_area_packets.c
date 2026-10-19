#include "fft/memory.h"
#include "fft/menu.h"

/* Clear the first draw area's width and install the default second draw area
 * (g_menu_draw_area_template). Many modules compile this text, WORLD three times (the option,
 * menu-screen and unit-panel copies, the first two never called), and one other copy is never
 * referenced (QUIRKS.md). */
void menu_init_scaled_draw_area_packets(world_scaled_draw_area_pair_t* data) {
    data->rects[0].w = 0;
    data->rects[1].h = 0;
    mem_copy_bytes_nonempty(&data->rects[1], &g_menu_draw_area_template, 8);
}
