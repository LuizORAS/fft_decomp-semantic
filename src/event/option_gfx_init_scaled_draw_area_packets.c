#include "fft/event_option.h"
#include "psx/types.h"

/* Clear the first draw area's width and install the default second draw area
 * (g_option_gfx_draw_area_template). Nothing references it (QUIRKS.md). */
void option_gfx_init_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* record) {
    record->rects[0].w = 0;
    record->rects[1].h = 0;
    battle_util_copy_bytes(&record->rects[1], &g_option_gfx_draw_area_template, 8);
}
