#include "fft/battle.h"
#include "psx/types.h"

/* Toggle the battle map zoom between "in" (target=1, action=2) and
   "out" (target=4, action=4), playing the corresponding zoom SFX. Any
   other current zoom target is left alone. */
void battle_camera_toggle_zoom(void) {
    if (g_battle_map_zoom_target == 1) {
        g_battle_camera_zoom_action = 4;
        g_battle_map_zoom_target = 4;
        main_sound_play_sfx(MAIN_SFX_CAMERA_ZOOM_OUT);
    } else if (g_battle_map_zoom_target == 4) {
        g_battle_map_zoom_target = 1;
        g_battle_camera_zoom_action = 2;
        main_sound_play_sfx(MAIN_SFX_CAMERA_ZOOM_IN);
    }
    g_battle_camera_zoom_increment = 4;
}
