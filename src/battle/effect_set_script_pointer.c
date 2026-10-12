#include "fft/battle_effect.h"
#include "psx/types.h"

void effect_set_script_pointer(s16 effect_id) {
    effect_file_header_t* header;

    header = g_effect_data_ptrs[effect_id];
    g_effect_script = (u8*)header + header->script;
}
