#include "fft/battle_effect.h"
#include "psx/types.h"

void effect_set_particle_system_data_pointer(s16 effect_id) {
    effect_file_header_t* header;

    header = g_effect_data_ptrs[effect_id];
    g_effect_geometry_table = (const effect_geometry_table_t*)((u8*)header + header->particle_system);
}
