#include "fft/battle.h"
#include "psx/types.h"

/* Toggle the camera tilt and play its sound. */
void battle_camera_toggle_tilt(void) {
    s32 target;

    target = g_battle_map_tilt_target;
    if (target == 1) {
        g_battle_camera_tilt_action = 2;
        g_battle_map_tilt_target = 2;
    } else if (target == 2) {
        g_battle_map_tilt_target = 1;
        g_battle_camera_tilt_action = 4;
    } else if (target == 3) {
        g_battle_camera_tilt_action = 4;
        g_battle_map_tilt_target = 4;
    } else if (target == 4) {
        g_battle_map_tilt_target = 1;
        g_battle_camera_tilt_action = 2;
    }
    g_battle_camera_tilt_increment = 1;
    main_sound_play_sfx(0x31);
}
