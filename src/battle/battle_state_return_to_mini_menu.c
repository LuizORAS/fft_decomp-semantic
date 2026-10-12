#include "fft/battle.h"

/* Leave the view of a unit picked from the AT list: enter MINI_MENU, remove the tile marks and
 * release OPTION.OUT's AT list thread (battle_menu_end_at_list_unit_view). */
void battle_state_return_to_mini_menu(void) {
    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_MINI_MENU;
    battle_target_tint_marked_tiles(BATTLE_TILE_MARK_NONE, 3);
    battle_menu_end_at_list_unit_view();
}
