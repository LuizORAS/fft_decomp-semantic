#include "fft/battle.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Out-and-back interpolation of three 16-bit values: the factor follows a whole
 * cosine wave over duration, so the value reaches to at half the duration and is
 * back at from when current reaches duration. */
void battle_effect_interpolate_svector_cos_q12(
    const SVECTOR* from, const SVECTOR* to, s32 duration, s32 current, SVECTOR* out) {
    battle_effect_interpolate_svector(from, to, ONE - rcos((current << 12) / duration), out);
}
