#include "fft/battle_effect.h"
#include "psx/types.h"

void effect_interpolate_vector_q13(const VECTOR* from, const VECTOR* to, s32 t, VECTOR* out) {
    effect_lerp_component_q13(from->vx, to->vx, t, &out->vx);
    effect_lerp_component_q13(from->vy, to->vy, t, &out->vy);
    effect_lerp_component_q13(from->vz, to->vz, t, &out->vz);
}
