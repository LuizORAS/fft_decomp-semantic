#include "fft/battle_effect.h"
#include "psx/types.h"

/* Linear counterpart of effect_interpolate_vector_q13_cos: t = current/duration in q13. */
void effect_interpolate_vector_q13_linear(
    const VECTOR* from, const VECTOR* to, s32 duration, s32 current, VECTOR* out) {
    effect_interpolate_vector_q13(from, to, (current << 13) / duration, out);
}
