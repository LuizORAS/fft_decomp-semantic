#include "fft/battle_effect.h"
#include "psx/types.h"

/* Interpolate one 16-bit value: out = from + (to - from) * t / 0x2000, so t is a fraction of 0x2000 (0
 * gives from, 0x2000 gives to). */
void battle_effect_interpolate_s16(s16 from, s16 to, s32 t, s16* out) {
    *out = from + (((to - from) * t) / (2 * ONE));
}
