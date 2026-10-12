#include "fft/battle_camera.h"

void battle_camera_restore_zoom(void) {
    copyVector(&g_battle_camera_zoom, &g_battle_camera_zoom_backup);
}
