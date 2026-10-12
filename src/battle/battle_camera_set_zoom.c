#include "fft/battle_camera.h"

void battle_camera_set_zoom(const VECTOR* zoom) {
    copyVector(&g_battle_camera_zoom, zoom);
}
