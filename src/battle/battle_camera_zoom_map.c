#include "fft/battle.h"
#include "psx/types.h"

void battle_camera_zoom_map(s32 mode) {
    switch (mode) {
    case 0:
        if (g_battle_camera_zoom.vx > ONE) {
            g_battle_map_zoom_target = BATTLE_MAP_ZOOM_IN;
            g_battle_camera_zoom_action = BATTLE_CAMERA_STEP_DECREASE;
            g_battle_camera_zoom_increment = 4;
            main_sound_play_sfx(MAIN_SFX_CAMERA_ZOOM_OUT);
            return;
        }
        if (g_battle_camera_zoom.vx < ONE) {
            g_battle_map_zoom_target = BATTLE_MAP_ZOOM_IN;
            g_battle_camera_zoom_action = BATTLE_CAMERA_STEP_INCREASE;
            g_battle_camera_zoom_increment = 4;
            main_sound_play_sfx(MAIN_SFX_CAMERA_ZOOM_IN);
            return;
        }
        return;
    case 1:
        if (g_battle_camera_zoom.vx > 0xC00) {
            g_battle_map_zoom_target = BATTLE_MAP_ZOOM_OUT;
            g_battle_camera_zoom_action = BATTLE_CAMERA_STEP_DECREASE;
            g_battle_camera_zoom_increment = 4;
            main_sound_play_sfx(MAIN_SFX_CAMERA_ZOOM_OUT);
            return;
        }
        if (g_battle_camera_zoom.vx < 0xC00) {
            g_battle_map_zoom_target = BATTLE_MAP_ZOOM_OUT;
            g_battle_camera_zoom_action = BATTLE_CAMERA_STEP_INCREASE;
            g_battle_camera_zoom_increment = 4;
            main_sound_play_sfx(MAIN_SFX_CAMERA_ZOOM_IN);
            return;
        }
        return;
    }
}
