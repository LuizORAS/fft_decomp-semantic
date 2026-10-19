#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_camera.h"
#include "fft/battle_target.h"

/* Enter UNIT_MOVING: save the unit's state (and its mount's) for an undo, set the step speed to
 * 0x2000, start at the path's first step and hide the map cursor. */
void battle_state_enter_unit_moving(void) {
    battle_unit_t* unit;

    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_UNIT_MOVING;
    battle_target_show_cursor_unit_panel();
    unit = battle_unit_get_casting();
    battle_unit_save_previous_state(unit);
    if (unit->mount_state == BATTLE_UNIT_MOUNT_STATE_MOUNT) {
        battle_unit_t* mount = battle_unit_get_by_runtime_id(unit->mount_partner_runtime_id);
        if (mount != 0) {
            battle_unit_save_previous_state(mount);
        }
    }
    unit->step_speed = 0x2000;
    unit->movement_path_offset = 0;
    g_battle_state_animation_continue_check = 0;
    battle_target_hide_cursor();
}
