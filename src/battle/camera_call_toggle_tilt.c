#include "fft/battle_camera.h"
#include "psx/pad.h"

/* Toggles the tilt when R2 is newly pressed and no tilt is running. */
void camera_call_toggle_tilt(void) {
    if (g_camera_tilt_action == CAMERA_STEP_NONE) {
        if ((g_main_input_pressed_buttons & PSX_PAD_R2) != 0) {
            camera_toggle_tilt();
        }
    }
}
