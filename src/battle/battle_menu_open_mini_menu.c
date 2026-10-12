#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_menu.h"

/* Leave the view of a unit picked from the AT list: enter the mini-menu state,
 * remove the tile marks and release OPTION.OUT's AT list thread, which waits on
 * g_option_menu_transition_finished. */
void battle_menu_open_mini_menu(void) {
    battle_state_stop_game_flow();
    g_battle_game_state = BATTLE_GAME_STATE_MINI_MENU;
    target_set_tile_background_color(BATTLE_TILE_MARK_NONE, 3);
    battle_menu_set_option_transition_finished();
}
