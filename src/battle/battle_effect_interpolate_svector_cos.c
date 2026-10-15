#include "fft/battle.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Eased interpolation of three halfwords: the factor follows half a cosine wave over duration, so the
 * value leaves from slowly and arrives at to slowly when current reaches duration. The effect camera's
 * keyframe moves use this curve and its linear and out-and-back forms (battle_camera_update_effect_tracks). */
void battle_effect_interpolate_svector_cos(const s16* from, const s16* to, s32 duration, s32 current, s16* out) {
    battle_effect_interpolate_svector(from, to, ONE - rcos((current << 11) / duration), out);
}
