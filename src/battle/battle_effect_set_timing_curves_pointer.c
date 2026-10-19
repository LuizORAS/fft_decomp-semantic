#include "fft/battle_effect.h"
#include "psx/types.h"

/* Point g_battle_effect_timing_curves at the timing curve section of effect file effect_id: the 4-bit
 * time scales battle_effect_get_timing_curve_value and battle_effect_get_animation_timing_curve_value read. */
void battle_effect_set_timing_curves_pointer(s16 effect_id) {
    effect_file_header_t* header;

    header = g_battle_effect_file_headers[effect_id];
    g_battle_effect_timing_curves = (const battle_effect_timing_curves_t*)((u8*)header + header->timing_curves);
}
