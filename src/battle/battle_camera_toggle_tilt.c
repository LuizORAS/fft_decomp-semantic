#include "fft/battle_camera.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* Toggle the camera tilt and play its sound. */
void battle_camera_toggle_tilt(void) {
    s32 target;

    target = g_battle_map_tilt_target;
    if (target == BATTLE_MAP_TILT_LOW) {
        g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_INCREASE;
        g_battle_map_tilt_target = BATTLE_MAP_TILT_HIGH;
    } else if (target == BATTLE_MAP_TILT_HIGH) {
        g_battle_map_tilt_target = BATTLE_MAP_TILT_LOW;
        g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_DECREASE;
    } else if (target == 3) {
        g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_DECREASE;
        g_battle_map_tilt_target = 4;
    } else if (target == 4) {
        g_battle_map_tilt_target = BATTLE_MAP_TILT_LOW;
        g_battle_camera_tilt_action = BATTLE_CAMERA_STEP_INCREASE;
    }
    g_battle_camera_tilt_increment = 1;
    main_sound_play_sfx(MAIN_SFX_CAMERA_TILT);
}
