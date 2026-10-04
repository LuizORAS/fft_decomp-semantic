#include "fft/battle.h"

/* ABILITY_PREVIEW_HELP: once the help window has closed, return to ABILITY_PREVIEW_HANDLING
 * with the d-pad panning the camera. */
void battle_state_handle_ability_preview_help_state(void) {
    if (battle_menu_is_still_building() != 2) {
        g_battle_menu_help_opening = 0;
        battle_state_enable_camera_pan();
        g_battle_game_state = BATTLE_GAME_STATE_ABILITY_PREVIEW_HANDLING;
    }
    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
}
