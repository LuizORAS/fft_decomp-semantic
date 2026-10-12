#include "fft/battle_effect.h"
#include "psx/types.h"

void effect_set_animation_curve_data_pointer(s16 effect_id) {
    effect_file_header_t* header;

    header = g_effect_data_ptrs[effect_id];
    g_effect_animation_curve_data = (effect_animation_curve_table_t*)((u8*)header + header->animation_curve);
}
