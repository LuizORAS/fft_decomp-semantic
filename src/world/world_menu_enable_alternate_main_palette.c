#include "fft/world.h"

/* Draw the main menu text window (world_menu_main_text_window_thread) in its alternate palette.
 * Nothing in the game calls it. */
void world_menu_enable_alternate_main_palette(void) {
    g_world_menu_use_alternate_main_palette = 1;
}
