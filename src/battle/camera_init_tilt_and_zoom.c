#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_map.h"
#include "fft/battle_script.h"
#include "psx/types.h"

/* Steps the camera tilt script variable by 0x10 per frame to the nearer of the
 * two resting pitches (0x12e below the midpoint 0x177, 0x1c0 from it up),
 * yielding the thread each frame, then stores the matching tilt target
 * (MAP_TILT_LOW or MAP_TILT_HIGH) and restores the zoomed-in scale
 * (MAP_ZOOM_IN, zoom variable ONE).
 *
 * BATTLE twin of world_camera_init_tilt_and_zoom. The event interpreter runs it
 * on the End instructions, as the last step before the event thread exits. */
void camera_init_tilt_and_zoom(void) {
    s32 current_tilt;
    s32 target_tilt;
    s32 tilt_step;
    s32 tilt_target_mode;

    current_tilt = g_script_variables[EVENT_SCRIPT_VAR_CAMERA_TILT];
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
            g_script_variables[EVENT_SCRIPT_VAR_CAMERA_TILT] = current_tilt;
            battle_thread_yield();
            current_tilt += tilt_step;
        }
        g_script_variables[EVENT_SCRIPT_VAR_CAMERA_TILT] = target_tilt;
    } else {
        while (current_tilt < target_tilt) {
            g_script_variables[EVENT_SCRIPT_VAR_CAMERA_TILT] = current_tilt;
            battle_thread_yield();
            current_tilt += tilt_step;
        }
        g_script_variables[EVENT_SCRIPT_VAR_CAMERA_TILT] = target_tilt;
    }

    g_map_tilt_target = tilt_target_mode + 1;
    g_map_zoom_target = MAP_ZOOM_IN;
    g_script_variables[EVENT_SCRIPT_VAR_CAMERA_ZOOM] = ONE;
}
