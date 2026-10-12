#include "fft/battle.h"

/* Leave the view of a unit picked from the AT list: enter the mini-menu state,
 * remove the tile marks and release OPTION.OUT's AT list thread, which waits on
 * g_battle_menu_at_list_unit_view_ended. */
void battle_menu_open_mini_menu(void) {
    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_MINI_MENU;
    battle_target_tint_marked_tiles(BATTLE_TILE_MARK_NONE, 3);
    battle_menu_end_at_list_unit_view();
}
