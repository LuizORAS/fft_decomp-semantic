#include "fft/battle.h"
#include "psx/types.h"

/* Draw the map cursor and its tile glow (g_battle_target_cursor_visible). */
void battle_target_show_cursor(void) {
    g_battle_target_cursor_visible = 1;
}
