#include "fft/battle.h"
#include "psx/types.h"

/* Stop drawing the map cursor and its tile glow; the cursor tile is still projected for the
 * camera. */
void battle_target_hide_cursor(void) {
    g_battle_target_cursor_visible = 0;
}
