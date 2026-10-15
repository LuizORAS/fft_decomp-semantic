#include "fft/battle.h"

/* Enter MINI_MENU from the free cursor and open the mini menu (AT, Unit List, Option). */
void battle_state_enter_mini_menu(void) {
    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_MINI_MENU;
    battle_menu_start_mini_menu_display_thread();
}
