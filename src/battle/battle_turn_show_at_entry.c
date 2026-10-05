#include "fft/battle.h"
#include "psx/types.h"

/* Enter ACTIVE_TURN to show the AT list entry chosen in the mini menu: the cursor on a unit's
 * turn, or the target panels of a charged action; then show the map cursor. */
void battle_turn_show_at_entry(s32 at_index) {
    s32 id;
    battle_unit_misc_data_t* unit;

    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_ACTIVE_TURN;
    id = battle_turn_get_at_entry_unit(at_index);
    if (id >= 0) {
        unit = battle_unit_get_misc_data_by_battle_id(id & 0xFF);
        if ((id & 0x100) != 0) {
            battle_target_gather_x_y_data_for_attacks(unit);
            battle_target_mark_action_area(&unit->battle_data->action_actor_id);
            battle_target_set_tile_background_color(7, 3);
        } else {
            battle_target_move_cursor_to_unit(unit);
            battle_target_set_tile_background_color(0, 0);
        }
    }
    battle_target_store_cursor_unit_name_and_data();
    battle_target_show_cursor();
}
