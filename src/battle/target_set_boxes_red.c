#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_camera.h"
#include "fft/battle_menu.h"

/* Start target selection for the source unit's previewed action and return its
 * ability_preview_phase (target_panels_e).
 *
 * An action with a target to choose enters the targeting-range state with the
 * range tiles shown and the cursor unit as preview target. An action without
 * targeting selects its tile at once. With no valid panels the idle action menu
 * reopens. */
s32 target_set_boxes_red(void) {
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_source_misc_data();
    unit->state_frame_counter = 0;
    switch (unit->ability_preview_phase) {
    case TARGET_PANELS_UNIT_OR_TILE:
    case TARGET_PANELS_TILE_ONLY:
        camera_disable_map_pan();
        g_battle_game_state = BATTLE_GAME_STATE_TARGETING_RANGE;
        target_set_tile_background_color(BATTLE_TILE_MARK_SHOW_RANGE, 2);
        target_store_cursor_unit_as_preview_target();
        if ((unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) != 0) {
            action_set_at_list_active();
        } else {
            action_clear_at_list_active();
        }
        break;
    case TARGET_PANELS_NO_TARGETING:
        camera_disable_map_pan();
        target_select_tile();
        target_store_cursor_unit_as_preview_target();
        break;
    case TARGET_PANELS_INVALID:
    case TARGET_PANELS_NONE:
        battle_menu_dispatch_idle_action_menu();
        break;
    }
    return unit->ability_preview_phase;
}
