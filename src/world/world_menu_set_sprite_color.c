#include "fft/world.h"

/* Set the display scripts' draw colour from an r, g, b byte triplet; NULL leaves it unchanged. */
void world_menu_set_sprite_color(u8* src) {
    if (src != 0) {
        g_world_menu_sprite_color.r = src[0];
        g_world_menu_sprite_color.g = src[1];
        g_world_menu_sprite_color.b = src[2];
    }
}
