#include "fft/battle_effect.h"
#include "psx/types.h"

/* Same as battle_effect_interpolate_s16 on a 32-bit value. */
void battle_effect_interpolate_s32(s32 from, s32 to, s32 t, s32* out) {
    *out = from + (to - from) * t / (2 * ONE);
}
