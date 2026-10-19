#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_camera.h"
#include "fft/battle_menu.h"
#include "fft/battle_target.h"

/* Open the tile choice for the acting unit's action by its range result (ability_preview_phase,
 * from battle_target_set_panels_for_action): 0 or 1 enter TARGETING_RANGE with the ability range
 * tinted and, for a player-controlled unit, the cursor shown; 2 needs no choice and goes straight to
 * battle_target_select_tile; -1 and 3 (invalid, or nothing in range) return to the action menu.
 * Returns the phase. */
s32 battle_target_begin_tile_selection(void) {
    battle_unit_t* unit;

    unit = battle_unit_get_source();
    unit->state_frame_counter = 0;
    switch (unit->ability_preview_phase) {
    case BATTLE_TARGET_PANELS_UNIT_OR_TILE:
    case BATTLE_TARGET_PANELS_TILE_ONLY:
        battle_camera_disable_map_pan();
        g_battle_game_state = BATTLE_GAME_STATE_TARGETING_RANGE;
        battle_target_tint_marked_tiles(BATTLE_TILE_MARK_SHOW_RANGE, 2);
        battle_target_show_cursor_target_panel();
        if ((unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) != 0) {
            battle_target_show_cursor();
        } else {
            battle_target_hide_cursor();
        }
        break;
    case BATTLE_TARGET_PANELS_NO_TARGETING:
        battle_camera_disable_map_pan();
        battle_target_select_tile();
        battle_target_show_cursor_target_panel();
        break;
    case BATTLE_TARGET_PANELS_INVALID:
    case BATTLE_TARGET_PANELS_NONE:
        battle_menu_dispatch_idle_action_menu();
        break;
    }
    return unit->ability_preview_phase;
}
