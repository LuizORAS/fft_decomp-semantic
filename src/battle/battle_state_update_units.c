#include "fft/battle.h"

/* Per-frame unit update most states run: rotation and animation (not while the status screen
 * is up), status bubbles and the map cursor (battle_target_update_cursor). */
void battle_state_update_units(void) {
    battle_gfx_update_all_unit_rotation_and_vectors();
    if (g_battle_menu_status_screen_selected != 1) {
        battle_unit_update_and_animate_units();
    }
    battle_gfx_update_status_bubbles_and_graphics();
    battle_target_update_cursor();
}
