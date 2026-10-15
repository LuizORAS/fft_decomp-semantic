#include "fft/battle.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Out-and-back interpolation of three halfwords: the factor follows a whole
 * cosine wave over duration, so the value reaches to at half the duration and is
 * back at from when current reaches duration. */
void battle_effect_interpolate_svector_out_and_back(
    const s16* from, const s16* to, s32 duration, s32 current, s16* out) {
    battle_effect_interpolate_svector(from, to, ONE - rcos((current << 12) / duration), out);
}
