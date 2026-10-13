#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_camera.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Enter the active-turn state and show entry at_index of the AT list.
 *
 * For a charged action (bit 0x100 of the entry) the cursor moves to the action's
 * target and the tiles it will hit are marked; for a unit's turn the cursor moves
 * to the unit and the tile marks are removed. An empty entry leaves the cursor
 * alone. */
void action_get_next_acting_unit(s32 at_index) {
    s32 id;
    battle_unit_t* unit;

    camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_ACTIVE_TURN;
    id = action_calculate_at_list_and_get_specific_unit_id(at_index);
    if (id >= 0) {
        unit = battle_unit_get_misc_data_by_battle_id(id & 0xFF);
        if ((id & 0x100) != 0) {
            target_move_cursor_to_action_target(unit);
            target_calculate_for_menu_types(&unit->battle_data->action_command);
            target_set_tile_background_color(BATTLE_TILE_MARK_SHOW_TARGETS, 3);
        } else {
            target_move_cursor_to_unit(unit);
            target_set_tile_background_color(BATTLE_TILE_MARK_NONE, 0);
        }
    }
    target_store_cursor_unit_name_and_data();
    action_set_at_list_active();
}
