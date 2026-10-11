#include "fft/battle.h"
#include "psx/types.h"

/* t is a fraction of 0x2000: 0 gives from and 0x2000 gives to. */
void battle_effect_interpolate_s16(s16 from, s16 to, s32 t, s16* out) {
    *out = from + (((to - from) * t) / 0x2000);
}
