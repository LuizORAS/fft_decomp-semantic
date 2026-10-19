#include "fft/battle_camera.h"

/* Put back the camera angles battle_camera_save_rotation kept (effect script opcode 0d). */
void battle_camera_restore_rotation(void) {
    copyVector(&g_battle_camera_angles, &g_battle_camera_saved_rotation);
}
