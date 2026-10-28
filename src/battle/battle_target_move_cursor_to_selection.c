#include "fft/battle.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Move the map cursor to the selected unit, or else to the tile in the unit's command
 * (command_state.cursor.target_panel), and when it moved refresh the panel for the state: the
 * cursor unit's in FREE_CURSOR, SELECT_MOVE_TILE and TARGET_SELECT, the preview target's in
 * TARGETING_RANGE. */
void battle_target_move_cursor_to_selection(const battle_unit_t* unit, const battle_unit_t* selected_unit) {
    s32 old_x;
    s32 old_z;
    s32 old_y;

    old_x = g_battle_cursor_x;
    old_z = g_battle_cursor_z;
    old_y = g_battle_cursor_y;
    if (selected_unit != 0) {
        battle_target_move_cursor_to_unit(selected_unit);
    } else {
        g_battle_cursor_x = unit->command_state.cursor.target_panel.vx;
        g_battle_cursor_z = unit->command_state.cursor.target_panel.vy;
        g_battle_cursor_y = unit->command_state.cursor.target_panel.vz;
    }
    if (((g_battle_cursor_x != old_x) | (g_battle_cursor_z != old_z) | (g_battle_cursor_y != old_y)) != 0) {
        switch (g_battle_game_state) {
        case BATTLE_GAME_STATE_FREE_CURSOR:
        case BATTLE_GAME_STATE_SELECT_MOVE_TILE:
        case BATTLE_GAME_STATE_TARGET_SELECT:
            battle_target_show_cursor_unit_panel();
            break;
        case BATTLE_GAME_STATE_TARGETING_RANGE:
            battle_target_show_cursor_target_panel();
            break;
        }
    }
}
