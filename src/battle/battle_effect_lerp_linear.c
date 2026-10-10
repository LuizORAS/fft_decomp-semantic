#include "fft/battle.h"

/* start + (end - start) * factor / 256: factor is a fraction of 256. */
s32 battle_effect_lerp_linear(s32 start, s32 end, s32 factor) {
    return ((end - start) * factor >> 8) + start;
}
