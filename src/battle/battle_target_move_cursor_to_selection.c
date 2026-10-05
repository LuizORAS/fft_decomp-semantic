#include "fft/battle.h"
#include "psx/types.h"

/* Move the map cursor to the selected unit, or else to the tile in the unit's command
 * (command_state.cursor.target_panel), and refresh the status panels when it moved. */
void battle_target_move_cursor_to_selection(battle_unit_misc_data_t* unit, battle_unit_misc_data_t* selected_unit) {
    s32 old_x;
    s32 old_z;
    s32 old_y;
    s32 nx;
    s32 nz;
    s32 ny;

    old_x = g_battle_cursor_x;
    old_z = g_battle_cursor_z;
    old_y = g_battle_cursor_y;
    if (selected_unit != 0) {
        battle_target_move_cursor_to_unit(selected_unit);
    } else {
        nx = unit->command_state.cursor.target_panel.vx;
        nz = unit->command_state.cursor.target_panel.vy;
        ny = unit->command_state.cursor.target_panel.vz;
        g_battle_cursor_x = nx;
        g_battle_cursor_z = nz;
        g_battle_cursor_y = ny;
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
