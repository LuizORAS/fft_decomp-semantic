#include "fft/battle_effect.h"
#include "psx/types.h"

/* Linear counterpart of effect_interpolate_svector_cos: t = current/duration in q13. */
void effect_interpolate_svector_linear(
    const SVECTOR* from, const SVECTOR* to, s32 duration, s32 current, SVECTOR* out) {
    effect_interpolate_svector(from, to, (current << 13) / duration, out);
}
