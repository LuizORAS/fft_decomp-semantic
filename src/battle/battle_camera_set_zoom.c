#include "fft/battle.h"

/* Set the camera's three zoom scales: effect script opcode 0e, the effect camera's zoom track and the event
 * camera's script variables. */
void battle_camera_set_zoom(const VECTOR* zoom) {
    g_battle_camera_zoom.vx = zoom->vx;
    g_battle_camera_zoom.vy = zoom->vy;
    g_battle_camera_zoom.vz = zoom->vz;
}
