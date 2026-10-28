#include "fft/menu.h"

/* Store the top-left text origin used by the menu text renderer. WORLD compiles this text too. */
void menu_set_text_origin(s32 x, s32 y) {
    g_menu_text_state.pen.rect.x = x;
    g_menu_text_state.pen.rect.y = y;
}
