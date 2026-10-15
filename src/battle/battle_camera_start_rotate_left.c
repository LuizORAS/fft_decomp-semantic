#include "fft/battle.h"
#include "psx/types.h"

/* Start a left turn to target_angle from the slowest speed (increment 1). L1
 * (battle_camera_handle_rotation_input) and battle_camera_request_rotation start turns this way. */
void battle_camera_start_rotate_left(s32 target_angle) {
    g_battle_camera_rotation_action = BATTLE_CAMERA_ROTATION_LEFT;
    g_battle_camera_rotation_increment = 1;
    battle_camera_set_left_turn_destination(target_angle);
}
