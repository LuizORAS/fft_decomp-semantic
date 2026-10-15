#include "fft/battle.h"

/* Same as battle_effect_lerp_linear, also storing the result in out: the four
 * battle_effect_interpolate_emitter_* routines call it for each component. */
s32 battle_effect_lerp_and_store(s32 start, s32 end, s32 factor, s32* out) {
    s32 result;

    result = ((end - start) * factor >> 8) + start;
    *out = result;
    return result;
}
