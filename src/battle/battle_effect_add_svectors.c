#include "fft/battle_effect.h"
#include "psx/libgte.h"

/* Add two SVECTORs into out (battle_camera_execute_angle_command). */
void battle_effect_add_svectors(const SVECTOR* first, const SVECTOR* second, SVECTOR* out) {
    out->vx = first->vx + second->vx;
    out->vy = first->vy + second->vy;
    out->vz = first->vz + second->vz;
}
