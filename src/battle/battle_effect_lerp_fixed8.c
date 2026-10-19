#include "fft/battle_effect.h"

/* Return start + (end - start) * factor / 256, rounded down (an arithmetic shift): factor is a fraction
 * of 256, 0 giving start and 256 giving end. The emitter spawner and the effect overlays' mesh, ribbon
 * and grid renderers step their values with it. */
s32 battle_effect_lerp_fixed8(s32 start, s32 end, s32 factor) {
    return ((end - start) * factor >> 8) + start;
}
