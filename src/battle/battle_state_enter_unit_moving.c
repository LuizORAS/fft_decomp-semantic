#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_camera.h"
#include "fft/battle_target.h"

/* Enter the unit-moving state: save the casting unit's tile, facing and mount
 * state for an undo (and its partner's when it is a mount) and start its path
 * from the first step at speed 0x2000. */
void battle_state_enter_unit_moving(void) {
    battle_unit_t* unit;

    camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_UNIT_MOVING;
    target_store_cursor_unit_name_and_data();
    unit = battle_unit_get_casting_misc_data();
    battle_unit_save_previous_state(unit);
    if (unit->mount_state == BATTLE_UNIT_MOUNT_STATE_MOUNT) {
        battle_unit_t* mount = battle_unit_get_misc_data_by_misc_id(unit->mount_partner_runtime_id);
        if (mount != 0) {
            battle_unit_save_previous_state(mount);
        }
    }
    unit->step_speed = 0x2000;
    unit->movement_path_offset = 0;
    g_battle_state_animation_continue_check = 0;
    action_clear_at_list_active();
}
