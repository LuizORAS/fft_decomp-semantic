#include "fft/world.h"
#include "psx/types.h"

/* Draw the main menu text window (world_main_menu_text_window_thread) in its alternate palette.
 * Nothing in the game calls it. */
void world_menu_enable_alternate_main_palette(void) {
    g_world_menu_use_alternate_main_palette = 1;
}
