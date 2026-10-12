#include "fft/battle_effect.h"

/* start + (end - start) * factor / 256, stored in out and returned: factor is a
 * fraction of 256. */
s32 battle_effect_lerp_and_store(s32 start, s32 end, s32 factor, s32* out) {
    s32 result;

    result = ((end - start) * factor >> 8) + start;
    *out = result;
    return result;
}
