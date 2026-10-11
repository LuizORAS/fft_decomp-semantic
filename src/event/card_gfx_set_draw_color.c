#include "fft/event_card.h"
#include "psx/types.h"

void card_gfx_set_draw_color(const rgb24_t* data) {
    g_card_text_color.r = data->r;
    g_card_text_color.g = data->g;
    g_card_text_color.b = data->b;
}
