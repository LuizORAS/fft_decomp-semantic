#include "fft/battle_camera.h"
#include "psx/pad.h"

/* L1 starts a quarter turn left and R1 one right, each with its sound, unless
 * a rotation is running. */
void battle_camera_handle_rotation_input(void) {
    if (g_battle_camera_rotation_action == BATTLE_CAMERA_ROTATION_NONE) {
        if (g_main_input_pressed_buttons & PSX_PAD_L1) {
            battle_camera_start_rotate_left((g_battle_camera_angles.vy & 0xFE00) - 0x400);
            main_sound_play_sfx_find_channel(MAIN_SFX_CAMERA_ROTATE_LEFT);
        }
        if (g_main_input_pressed_buttons & PSX_PAD_R1) {
            battle_camera_start_rotate_right((g_battle_camera_angles.vy & 0xFE00) + 0x400);
            main_sound_play_sfx_find_channel(MAIN_SFX_CAMERA_ROTATE_RIGHT);
        }
    }
}
