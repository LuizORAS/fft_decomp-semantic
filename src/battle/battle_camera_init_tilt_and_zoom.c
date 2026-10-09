#include "fft/battle.h"
#include "psx/types.h"

void battle_camera_init_tilt_and_zoom(void) {
    s32 current_tilt;
    s32 target_tilt;
    s32 tilt_step;
    s32 tilt_target_mode;

    current_tilt = g_battle_script_variables[BATTLE_CAMERA_TILT_WORD_INDEX];
    tilt_target_mode = 0;
    if (current_tilt < 0x12e) {
        tilt_step = 0x10;
        target_tilt = 0x12e;
    } else if (current_tilt < 0x177) {
        tilt_step = -0x10;
        target_tilt = 0x12e;
    } else if (current_tilt < 0x1c0) {
        tilt_target_mode = 1;
        tilt_step = 0x10;
        target_tilt = 0x1c0;
    } else {
        tilt_target_mode = 1;
        tilt_step = -0x10;
        target_tilt = 0x1c0;
    }

    if (tilt_step < 0) {
        while (target_tilt < current_tilt) {
            g_battle_script_variables[BATTLE_CAMERA_TILT_WORD_INDEX] = current_tilt;
            battle_thread_yield();
            current_tilt += tilt_step;
        }
        g_battle_script_variables[BATTLE_CAMERA_TILT_WORD_INDEX] = target_tilt;
    } else {
        while (current_tilt < target_tilt) {
            g_battle_script_variables[BATTLE_CAMERA_TILT_WORD_INDEX] = current_tilt;
            battle_thread_yield();
            current_tilt += tilt_step;
        }
        g_battle_script_variables[BATTLE_CAMERA_TILT_WORD_INDEX] = target_tilt;
    }

    g_battle_map_tilt_target = tilt_target_mode + 1;
    g_battle_map_zoom_target = 1;
    g_battle_script_variables[BATTLE_CAMERA_ZOOM_WORD_INDEX] = ONE;
}
