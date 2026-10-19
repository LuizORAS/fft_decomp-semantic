#include "fft/battle_camera.h"
#include "fft/battle_map.h"

/* Switch the zoom target for L2 between in (scale ONE) and out (0xc00), start the move and play its
 * sound. Any other target is left alone, but the zoom increment still restarts at 4. */
void battle_camera_toggle_zoom(void) {
    if (g_battle_camera_zoom_target == BATTLE_CAMERA_ZOOM_IN) {
        g_battle_camera_zoom_action = BATTLE_CAMERA_STEP_DECREASE;
        g_battle_camera_zoom_target = BATTLE_CAMERA_ZOOM_OUT;
        main_sound_play_sfx(MAIN_SFX_CAMERA_ZOOM_OUT);
    } else if (g_battle_camera_zoom_target == BATTLE_CAMERA_ZOOM_OUT) {
        g_battle_camera_zoom_target = BATTLE_CAMERA_ZOOM_IN;
        g_battle_camera_zoom_action = BATTLE_CAMERA_STEP_INCREASE;
        main_sound_play_sfx(MAIN_SFX_CAMERA_ZOOM_IN);
    }
    g_battle_camera_zoom_increment = 4;
}
