#include "fft/battle.h"
#include "psx/types.h"

/* Toggle the camera tilt and play its sound. The sound call passes a second
 * argument its one-parameter callee ignores; `sound_arg` is unset when the
 * target is 1, so that argument is then whatever the caller left in $a1. */
void battle_camera_toggle_tilt(void) {
    s32 target;
    s32 sound_arg;

    target = g_battle_map_tilt_target;
    if (target == 1) {
        g_battle_camera_tilt_action = 2;
        g_battle_map_tilt_target = 2;
    } else {
        sound_arg = 2;
        if (target == 2) {
            g_battle_map_tilt_target = 1;
            g_battle_camera_tilt_action = 4;
        } else if (target == 3) {
            g_battle_camera_tilt_action = 4;
            g_battle_map_tilt_target = 4;
        } else if (target == 4) {
            g_battle_map_tilt_target = 1;
            g_battle_camera_tilt_action = 2;
        }
    }
    g_battle_camera_tilt_increment = 1;
    ((void (*)(s32, s32))main_sound_play_sfx)(0x31, sound_arg);
}
