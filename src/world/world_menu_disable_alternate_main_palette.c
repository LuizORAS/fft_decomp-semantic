#include "fft/world.h"

/* Draw the main menu text window in its normal palette again. Nothing in the game calls it. */
void world_menu_disable_alternate_main_palette(void) {
    g_world_menu_use_alternate_main_palette = 0;
}
