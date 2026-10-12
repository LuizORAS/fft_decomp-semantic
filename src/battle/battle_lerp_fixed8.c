#include "fft/battle_effect.h"

/* Linear interpolation with an eight-bit fractional factor (256 = 1.0).
 * The arithmetic right shift rounds negative products down. */
s32 battle_lerp_fixed8(s32 start, s32 end, s32 factor) {
    return ((end - start) * factor >> 8) + start;
}
