#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_suppress_height_display. */
void world_menu_suppress_height_display(void) {
    g_world_menu_height_display_suppressed = 1;
}
