#include "fft/battle.h"
#include "psx/types.h"

/* Start a tilt toward the low pitch 0x12e (mode 0) or the high pitch 0x1c0
 * (mode 1) from whichever side the pitch is on. No retail code calls it; the
 * R2 input goes through battle_camera_toggle_tilt. */
void battle_camera_start_tilt(s32 mode) {
    switch (mode) {
    case 0:
        if (g_battle_camera_angles.vx >= 0x12F) {
            g_battle_camera_tilt_target = BATTLE_CAMERA_TILT_LOW;
            g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_DECREASE;
            g_battle_camera_tilt_increment = 1;
            main_sound_play_sfx(MAIN_SFX_CAMERA_TILT);
        } else if (g_battle_camera_angles.vx < 0x12E) {
            g_battle_camera_tilt_target = BATTLE_CAMERA_TILT_LOW;
            g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_INCREASE;
            g_battle_camera_tilt_increment = 1;
            main_sound_play_sfx(MAIN_SFX_CAMERA_TILT);
        }
        break;
    case 1:
        if (g_battle_camera_angles.vx >= 0x1C1) {
            g_battle_camera_tilt_target = BATTLE_CAMERA_TILT_HIGH;
            g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_DECREASE;
            g_battle_camera_tilt_increment = mode;
            main_sound_play_sfx(MAIN_SFX_CAMERA_TILT);
        } else if (g_battle_camera_angles.vx < 0x1C0) {
            g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_INCREASE;
            g_battle_camera_tilt_target = BATTLE_CAMERA_TILT_HIGH;
            g_battle_camera_tilt_increment = mode;
            main_sound_play_sfx(MAIN_SFX_CAMERA_TILT);
        }
        break;
    }
}
