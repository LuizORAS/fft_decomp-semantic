#include "fft/battle_action.h"

/* Per-frame cursor update: project the cursor tile to the screen, and while
 * g_action_at_list_active is set also draw the selection cursor and the
 * tile glow. */
void target_calculate_tile_coords_and_glow_from_at_list(void) {
    SVECTOR secondary;
    SVECTOR tertiary;
    VECTOR primary;

    if (g_action_at_list_active != 0) {
        target_calculate_tile_coords_with_cursor_glow();
        return;
    }
    target_project_cursor_tile_to_screen(&primary, &secondary, &tertiary);
}
