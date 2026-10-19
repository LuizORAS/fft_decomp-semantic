#include "fft/battle_camera.h"

/* Put back the camera position battle_camera_save_current_real_coords kept (effect script opcode 0a). */
void battle_camera_restore_current_real_coords(void) {
    copyVector(&g_battle_camera_current_real_coords, &g_battle_camera_saved_real_coords);
}
