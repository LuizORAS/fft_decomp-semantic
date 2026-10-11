#include "fft/battle.h"

void battle_camera_set_zoom(const VECTOR* zoom) {
    copyVector(&g_battle_camera_zoom, zoom);
}
