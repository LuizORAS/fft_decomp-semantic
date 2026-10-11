#include "fft/event_card.h"
#include "psx/types.h"

rgb24_t* card_gfx_get_draw_color(void) {
    return &g_card_text_color;
}
