#include "fft/battle.h"
#include "psx/types.h"

/* Start an automatic turn of angle yaw units (0x400 is a quarter turn;
 * negative turns left) with its sound, unless a rotation is running. */
void battle_camera_request_rotation(s32 angle) {
    if (g_battle_camera_rotation_action == BATTLE_CAMERA_ROTATION_NONE) {
        if (angle < 0) {
            battle_camera_start_rotate_left((g_battle_camera_angles.vy & 0xFE00) + angle);
            main_sound_play_sfx_find_channel(MAIN_SFX_CAMERA_ROTATE_LEFT);
        } else {
            battle_camera_start_rotate_right((g_battle_camera_angles.vy & 0xFE00) + angle);
            main_sound_play_sfx_find_channel(MAIN_SFX_CAMERA_ROTATE_RIGHT);
        }
    }
}
