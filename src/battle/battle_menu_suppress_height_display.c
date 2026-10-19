#include "fft/battle_menu.h"

/* Stop the selected tile's panel from drawing the tile height
 * (battle_menu_selected_tile_info_display_thread); battle_menu_enable_height_display undoes it. */
void battle_menu_suppress_height_display(void) {
    g_battle_menu_height_display_suppressed = 1;
}
