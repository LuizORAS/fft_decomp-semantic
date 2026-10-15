#include "psx/libgte.h"
#include "psx/types.h"

/* Widen three halfwords into a VECTOR: the effect camera's keyframe moves read a keyframe's position or
 * zoom this way (battle_camera_start_parent_keyframe_moves, battle_camera_start_child_keyframe_moves). */
void battle_effect_convert_svector_to_vector(s16* src, VECTOR* dst) {
    dst->vx = src[0];
    dst->vy = src[1];
    dst->vz = src[2];
}
