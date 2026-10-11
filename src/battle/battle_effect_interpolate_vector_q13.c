#include "fft/battle.h"
#include "psx/types.h"

void battle_effect_interpolate_vector_q13(const VECTOR* from, const VECTOR* to, s32 t, VECTOR* out) {
    battle_effect_lerp_component_q13(from->vx, to->vx, t, &out->vx);
    battle_effect_lerp_component_q13(from->vy, to->vy, t, &out->vy);
    battle_effect_lerp_component_q13(from->vz, to->vz, t, &out->vz);
}
