#include "fft/event_card.h"
#include "fft/menu.h"

/* Copy the three RGB bytes at `data` into CARD's draw colour (g_menu_text_color). */
void card_gfx_set_draw_color(const rgb24_t* data) {
    g_menu_text_color.r = data->r;
    g_menu_text_color.g = data->g;
    g_menu_text_color.b = data->b;
}
