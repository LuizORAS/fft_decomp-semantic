#include "fft/world.h"
#include "psx/types.h"

/* Set the scroll direction and threshold of the main menu text window
 * (world_main_menu_text_window_thread). Nothing in the game calls it. */
void world_menu_set_main_scroll_parameters(s32 direction, s32 threshold) {
    g_world_menu_main_scroll_direction = direction;
    g_world_menu_main_scroll_threshold = threshold;
}
