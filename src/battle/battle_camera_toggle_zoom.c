#include "fft/battle.h"
#include "psx/types.h"

/* Toggle the battle map zoom between "in" (target=1, action=2) and
   "out" (target=4, action=4), playing the corresponding zoom SFX. Any
   other current zoom target is left alone. */
void battle_camera_toggle_zoom(void) {
    if (g_battle_map_zoom_target == BATTLE_MAP_ZOOM_IN) {
        g_battle_camera_zoom_action = BATTLE_CAMERA_STEP_DECREASE;
        g_battle_map_zoom_target = BATTLE_MAP_ZOOM_OUT;
        main_sound_play_sfx(MAIN_SFX_CAMERA_ZOOM_OUT);
    } else if (g_battle_map_zoom_target == BATTLE_MAP_ZOOM_OUT) {
        g_battle_map_zoom_target = BATTLE_MAP_ZOOM_IN;
        g_battle_camera_zoom_action = BATTLE_CAMERA_STEP_INCREASE;
        main_sound_play_sfx(MAIN_SFX_CAMERA_ZOOM_IN);
    }
    g_battle_camera_zoom_increment = 4;
}
