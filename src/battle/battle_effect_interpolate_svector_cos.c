#include "fft/battle_effect.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Eased interpolation of three 16-bit values: the factor follows half a cosine wave
 * over duration, so the value leaves from slowly, and arrives at to slowly when
 * current reaches duration. */
void battle_effect_interpolate_svector_cos(
    const SVECTOR* from, const SVECTOR* to, s32 duration, s32 current, SVECTOR* out) {
    battle_effect_interpolate_svector(from, to, ONE - rcos((current << 11) / duration), out);
}
