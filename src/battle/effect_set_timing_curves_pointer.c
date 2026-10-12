#include "fft/battle_effect.h"
#include "psx/types.h"

/* A zero section offset binds the pointer to the file header. */
void effect_set_timing_curves_pointer(s16 effect_id) {
    effect_file_header_t* header;

    header = g_effect_data_ptrs[effect_id];
    g_effect_timing_curves = (effect_timing_curves_t*)((u8*)header + header->timing_curves);
}
