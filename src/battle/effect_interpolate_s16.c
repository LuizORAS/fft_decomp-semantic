#include "fft/battle_effect.h"
#include "psx/types.h"

/* t ranges from 0 at from to 2 * ONE at to. */
void effect_interpolate_s16(s16 from, s16 to, s32 t, s16* out) {
    *out = from + (((to - from) * t) / (2 * ONE));
}
