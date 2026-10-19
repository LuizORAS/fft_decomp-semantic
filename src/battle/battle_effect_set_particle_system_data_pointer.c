#include "fft/battle_effect.h"
#include "psx/types.h"

/* Point g_battle_effect_geometry_table at the particle-system section of effect file effect_id: its
 * gravity, its inertia threshold and its emitter or mesh records. */
void battle_effect_set_particle_system_data_pointer(s16 effect_id) {
    effect_file_header_t* header;

    header = g_battle_effect_file_headers[effect_id];
    g_battle_effect_geometry_table = (const effect_geometry_table_t*)((u8*)header + header->particle_system);
}
