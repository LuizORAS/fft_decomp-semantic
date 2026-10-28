#include "fft/world.h"

/* Return a pointer to the display scripts' draw color (its r, g, b bytes). */
rgb24_t* world_menu_get_sprite_color(void) {
    return &g_world_menu_sprite_color;
}
