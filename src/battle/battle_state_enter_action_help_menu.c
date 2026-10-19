#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_menu.h"

/* Enter ACTION_HELP_MENU and start the help menu (battle_menu_start_help_menu_thread). */
void battle_state_enter_action_help_menu(void) {
    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_ACTION_HELP_MENU;
    g_battle_menu_help_opening = 1;
    battle_menu_start_help_menu_thread();
}
