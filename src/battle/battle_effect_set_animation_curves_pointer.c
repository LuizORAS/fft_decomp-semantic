#include "fft/battle.h"
#include "psx/types.h"

/* Point g_battle_effect_animation_curves at the animation curve section of effect file effect_id: the
 * per-frame factors that move an emitter's values from their start to their end, tint the particles and set
 * their homing strength. */
void battle_effect_set_animation_curves_pointer(s16 effect_id) {
    effect_file_header_t* header;

    header = g_battle_effect_file_headers[effect_id];
    g_battle_effect_animation_curves = (effect_animation_curve_t*)((u8*)header + header->animation_curve);
}
