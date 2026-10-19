#include "fft/battle.h"
#include "fft/battle_effect.h"
#include "psx/types.h"

/* Set the three effect camera modes (rotation, position, zoom) idle, zero two 32-bit values nothing reads
 * (D_801B8B18, D_801B8B1C) and seed the camera snapshots from the live camera. battle_effect_init_system calls
 * it. */
void battle_effect_init_camera(void) {
    s32 pad[2];
    g_battle_effect_camera_rotation_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
    g_battle_effect_camera_position_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
    g_battle_effect_camera_zoom_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
    D_801B8B1C = 0;
    D_801B8B18 = 0;
    battle_effect_copy_camera_angles_and_world_coords();
    (void)pad;
}
