#include "fft/battle.h"
#include "psx/types.h"

/* Let the selected tile's panel draw the tile height again. */
void battle_menu_enable_height_display(void) {
    g_battle_menu_height_display_suppressed = 0;
}
