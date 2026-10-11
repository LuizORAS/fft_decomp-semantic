#include "fft/battle.h"
#include "psx/types.h"

/* Linear counterpart of battle_effect_interpolate_svector_cos: t = current/duration in q13. */
void battle_effect_interpolate_svector_linear(
    const SVECTOR* from, const SVECTOR* to, s32 duration, s32 current, SVECTOR* out) {
    battle_effect_interpolate_svector(from, to, (current << 13) / duration, out);
}
