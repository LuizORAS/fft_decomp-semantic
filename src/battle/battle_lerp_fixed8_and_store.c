#include "fft/battle_effect.h"

/* Store and return the interpolation with an eight-bit fractional factor.
 * The arithmetic right shift rounds negative products down. */
s32 battle_lerp_fixed8_and_store(s32 start, s32 end, s32 factor, s32* out) {
    s32 result;

    result = ((end - start) * factor >> 8) + start;
    *out = result;
    return result;
}
