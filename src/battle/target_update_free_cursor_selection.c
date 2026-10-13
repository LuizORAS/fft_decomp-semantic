#include "fft/battle.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Move the cursor while a target is being chosen: onto selected_unit when one is
 * given, otherwise to the tile stored in the unit's cursor command state.
 *
 * When the cursor tile changes, the unit panels are refreshed for the current
 * game state: the cursor unit's panel in the free-cursor, close-move-help and
 * target-select states, the preview target's panel in the targeting-range state. */
void target_update_free_cursor_selection(
    const battle_unit_misc_data_t* unit, const battle_unit_misc_data_t* selected_unit) {
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
        target_move_cursor_to_unit(selected_unit);
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
        case BATTLE_GAME_STATE_CLOSE_MOVE_HELP:
        case BATTLE_GAME_STATE_TARGET_SELECT:
            target_store_cursor_unit_name_and_data();
            break;
        case BATTLE_GAME_STATE_TARGETING_RANGE:
            target_store_cursor_unit_as_preview_target();
            break;
        }
    }
}
