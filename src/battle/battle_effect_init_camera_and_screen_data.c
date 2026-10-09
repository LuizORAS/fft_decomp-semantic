#include "fft/battle.h"
#include "psx/types.h"

void battle_effect_init_camera_and_screen_data(void) {
    s32 pad[2];
    g_battle_effect_camera_rotation_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
    g_battle_effect_camera_position_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
    g_battle_effect_camera_zoom_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
    D_801B8B1C = 0;
    D_801B8B18 = 0;
    battle_effect_copy_camera_angles_and_screen_coords();
    (void)pad;
}
