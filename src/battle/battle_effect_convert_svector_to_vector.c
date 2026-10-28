#include "fft/battle_effect.h"
#include "psx/libgte.h"

/* Widen three 16-bit values into a VECTOR: the effect camera's keyframe moves read a keyframe's position or
 * zoom this way (battle_camera_start_parent_keyframe_moves, battle_camera_start_child_keyframe_moves). */
void battle_effect_convert_svector_to_vector(const vec3s16_t* src, VECTOR* dst) {
    dst->vx = src->x;
    dst->vy = src->y;
    dst->vz = src->z;
}
