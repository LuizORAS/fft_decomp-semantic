#include "fft/battle.h"

void battle_menu_open_mini_menu(void) {
    battle_state_stop_game_flow();
    g_battle_game_state = BATTLE_GAME_STATE_MINI_MENU;
    battle_target_set_tile_background_color(BATTLE_TILE_MARK_NONE, 3);
    battle_menu_set_option_transition_finished();
}
