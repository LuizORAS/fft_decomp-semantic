#include "fft/battle.h"
#include "psx/pad.h"
#include "psx/types.h"

void battle_camera_call_zoom_map(void) {
    if (g_battle_camera_zoom_action == 0) {
        if ((g_main_input_pressed_buttons & PSX_PAD_L2) != 0) {
            battle_camera_toggle_zoom();
        }
    }
}
