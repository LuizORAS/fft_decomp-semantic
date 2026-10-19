#include "fft/world.h"

/* Tint the menu windows, icon lists and the zodiac backdrop with one r, g, b colour, which WLDCORE's
 * fades and transitions and the screens' open requests set. */
void world_menu_set_brightness(s32 red, s32 green, s32 blue) {
    g_world_menu_color_red = red;
    g_world_menu_color_green = green;
    g_world_menu_color_blue = blue;
}
