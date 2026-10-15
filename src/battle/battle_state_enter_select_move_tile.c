#include "fft/battle.h"
#include "psx/types.h"

/* Start choosing a move destination: recompute the casting unit's reachable tiles, mark them and
 * enter SELECT_MOVE_TILE with the cursor unit's panel; with no reachable tile, enter the move-range
 * exception instead. The map cursor shows for a player-controlled unit and hides otherwise. */
void battle_state_enter_select_move_tile(void) {
    battle_unit_misc_data_t* unit;
    battle_unit_misc_data_t* casting;

    battle_state_disable_camera_pan();
    unit = battle_unit_get_source_misc_data();
    casting = battle_unit_get_casting_misc_data();
    if ((unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) != 0) {
        battle_target_show_cursor();
    } else {
        battle_target_hide_cursor();
    }

    if (battle_move_set_reachable_tiles(
            casting->battle_data->misc_unit_id, casting->map_x, casting->map_y, casting->map_z)
        > 0) {
        battle_target_tint_marked_tiles(BATTLE_TILE_MARK_SHOW_MOVE, 1);
        g_battle_game_state = BATTLE_GAME_STATE_SELECT_MOVE_TILE;
        casting->state_frame_counter = 0;
        battle_target_show_cursor_unit_panel();
    } else {
        battle_state_enter_move_range_exception();
    }
}
