#include "fft/event_option.h"

/* Reset the mutable fields and restore the embedded menu template. */
void option_gfx_init_scaled_draw_area_packets(scaled_draw_area_pair_t* record) {
    record->rects[0].w = 0;
    record->rects[1].h = 0;
    battle_copy_bytes(&record->rects[1], &g_option_gfx_draw_area_template, 8);
}
