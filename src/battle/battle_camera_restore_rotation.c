#include "fft/battle_camera.h"

void battle_camera_restore_rotation(void) {
    copyVector(&g_battle_camera_render_state, &g_battle_camera_rotation_backup);
}
