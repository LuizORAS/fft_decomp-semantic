#include "fft/battle.h"
#include "psx/types.h"

/* Project the cursor tile to the screen and, while the cursor is visible, draw the cursor and
 * its tile glow. */
void battle_target_update_cursor(void) {
    SVECTOR secondary;
    SVECTOR tertiary;
    VECTOR primary;

    if (g_battle_target_cursor_visible != 0) {
        battle_target_draw_cursor_and_tile_glow();
        return;
    }
    battle_target_project_cursor_tile_to_screen(&primary, &secondary, &tertiary);
}
