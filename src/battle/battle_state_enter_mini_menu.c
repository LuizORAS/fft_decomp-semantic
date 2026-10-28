#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_menu.h"

/* Enter MINI_MENU from the free cursor and open the mini menu (AT, Unit List, Option). */
void battle_state_enter_mini_menu(void) {
    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_MINI_MENU;
    battle_menu_start_mini_menu_display_thread();
}
