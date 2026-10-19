#include "fft/battle_camera.h"
#include "psx/pad.h"

/* Toggles the tilt when R2 is newly pressed and no tilt is running. */
void battle_camera_handle_tilt_input(void) {
    if (g_battle_camera_tilt_action == BATTLE_CAMERA_STEP_NONE) {
        if ((g_main_input_pressed_buttons & PSX_PAD_R2) != 0) {
            battle_camera_toggle_tilt();
        }
    }
}
