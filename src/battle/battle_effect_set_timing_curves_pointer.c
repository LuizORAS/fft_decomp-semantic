#include "fft/battle.h"
#include "psx/types.h"

void battle_effect_set_timing_curves_pointer(s16 effect_id) {
    effect_file_header_t* header;

    header = g_battle_effect_data_ptrs[effect_id];
    g_battle_effect_timing_curves = (u8*)header + header->timing_curves;
}
