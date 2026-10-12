#include "fft/battle_effect.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Eased interpolation of three 32-bit components: the factor follows half a cosine wave over
 * duration, so the value leaves from slowly, and arrives at to slowly when
 * current reaches duration. */
void effect_interpolate_vector_q13_cos(const VECTOR* from, const VECTOR* to, s32 duration, s32 current, VECTOR* out) {
    effect_interpolate_vector_q13(from, to, ONE - rcos((current << 11) / duration), out);
}
