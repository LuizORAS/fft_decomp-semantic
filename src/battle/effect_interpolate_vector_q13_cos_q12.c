#include "fft/battle_effect.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Out-and-back interpolation of three 32-bit components: the factor follows a whole cosine
 * wave over duration, so the value reaches to at half the duration and is back at
 * from when current reaches duration. */
void effect_interpolate_vector_q13_cos_q12(
    const VECTOR* from, const VECTOR* to, s32 duration, s32 current, VECTOR* out) {
    effect_interpolate_vector_q13(from, to, ONE - rcos((current * ONE) / duration), out);
}
