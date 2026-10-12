#include "fft/battle.h"

/* Enter MINI_MENU_HELP and start the help menu (battle_menu_start_help_menu_thread). */
void battle_state_enter_mini_menu_help(void) {
    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_MINI_MENU_HELP;
    g_battle_menu_help_opening = 1;
    battle_menu_start_help_menu_thread();
}
