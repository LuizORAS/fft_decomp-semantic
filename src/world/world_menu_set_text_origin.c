#include "fft/menu.h"
#include "fft/world.h"
#include "psx/types.h"

/* Store the top-left text origin used by the WORLD menu renderer. */
void world_menu_set_text_origin(s32 x, s32 y) {
    g_menu_text_state.pen.rect.x = x;
    g_menu_text_state.pen.rect.y = y;
}
