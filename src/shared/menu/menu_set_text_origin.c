#include "fft/menu.h"

/* Set the screen origin used by menu text glyphs. */
void menu_set_text_origin(s32 x, s32 y) {
    g_menu_text_state.pen.rect.x = x;
    g_menu_text_state.pen.rect.y = y;
}
