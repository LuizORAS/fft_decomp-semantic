#include "fft/battle.h"
#include "psx/types.h"

/* Switch the tilt target for R2 between low (pitch 0x12e) and high (0x1c0), start the move and play its
 * sound. Targets 3 and 4, which no BATTLE code gives a pitch, lead to each other and back to low. */
void battle_camera_toggle_tilt(void) {
    s32 target;

    target = g_battle_camera_tilt_target;
    if (target == BATTLE_CAMERA_TILT_LOW) {
        g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_INCREASE;
        g_battle_camera_tilt_target = BATTLE_CAMERA_TILT_HIGH;
    } else if (target == BATTLE_CAMERA_TILT_HIGH) {
        g_battle_camera_tilt_target = BATTLE_CAMERA_TILT_LOW;
        g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_DECREASE;
    } else if (target == 3) {
        g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_DECREASE;
        g_battle_camera_tilt_target = 4;
    } else if (target == 4) {
        g_battle_camera_tilt_target = BATTLE_CAMERA_TILT_LOW;
        g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_INCREASE;
    }
    g_battle_camera_tilt_increment = 1;
    main_sound_play_sfx(MAIN_SFX_CAMERA_TILT);
}
