#include "fft/battle_camera.h"

/* Put back the zoom battle_camera_save_zoom kept (effect script opcode 24). */
void battle_camera_restore_zoom(void) {
    copyVector(&g_battle_camera_zoom, &g_battle_camera_saved_zoom);
}
