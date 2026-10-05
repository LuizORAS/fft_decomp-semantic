#include "fft/battle.h"

void battle_menu_open_mini_menu(void) {
    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_MINI_MENU;
    battle_target_tint_marked_tiles(BATTLE_TARGET_TINT_RESET, 3);
    battle_menu_set_option_transition_finished();
}
