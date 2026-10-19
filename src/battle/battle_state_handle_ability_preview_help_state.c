#include "fft/battle.h"
#include "fft/battle_camera.h"

/* ABILITY_PREVIEW_HELP: once the help window has closed, return to ABILITY_PREVIEW_HANDLING
 * with the d-pad panning the camera. */
void battle_state_handle_ability_preview_help_state(void) {
    if (battle_menu_get_thread_activity() != 2) {
        g_battle_menu_help_opening = 0;
        battle_camera_enable_map_pan();
        g_battle_game_state = BATTLE_GAME_STATE_ABILITY_PREVIEW_HANDLING;
    }
    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
}
