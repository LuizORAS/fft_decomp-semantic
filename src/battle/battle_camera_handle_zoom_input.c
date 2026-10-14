#include "fft/battle.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Toggles the zoom when L2 is newly pressed and no zoom is running. */
void battle_camera_handle_zoom_input(void) {
    if (g_battle_camera_zoom_action == BATTLE_CAMERA_STEP_NONE) {
        if ((g_main_input_pressed_buttons & PSX_PAD_L2) != 0) {
            battle_camera_toggle_zoom();
        }
    }
}
