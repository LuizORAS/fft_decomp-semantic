#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_target.h"

/* Enter TARGET_DISPLAY: move the cursor to the casting unit's Auto-Battle target, turning the
 * camera to it when its tile is hidden, and store its names for the menus. */
void battle_state_enter_target_display(void) {
    battle_stats_t* cast;
    battle_unit_t* unit;
    battle_stats_t* target;

    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_TARGET_DISPLAY;
    cast = battle_unit_get_casting()->stats;
    unit = battle_unit_get_by_battle_id(cast->auto_battle_target);
    battle_target_move_cursor_to_unit(unit);
    battle_camera_rotate_when_unit_tile_not_visible(unit);
    if (unit != 0) {
        target = unit->stats;
        if (target != 0) {
            battle_unit_start_view_thread(UNIT_VIEW_MODE_UNIT_SUMMARY, target->battle_id, 0);
        }
    }
}
