#include "fft/battle_effect.h"
#include "psx/types.h"

/* Points the model globals at the sections of a projectile model's single
 * object, rewinds its primitive stream and returns its primitive count. */
s32 battle_effect_init_model_sections(psyq_tmd_header_t* resource, s32 unused_1) {
    g_battle_effect_model_header = resource->objects;
    /* TMD offsets are byte counts from the object table; 32-bit indexing keeps signed rounding. */
    g_battle_effect_model_vertices = (SVECTOR*)&((u32*)resource->objects)[(s32)resource->objects[0].vertex_offset / 4];
    D_801B8A34 = (SVECTOR*)&((u32*)resource->objects)[(s32)resource->objects[0].normal_offset / 4];
    g_battle_effect_model_commands
        = (psyq_tmd_packet_t*)&((u32*)resource->objects)[(s32)resource->objects[0].primitive_offset / 4];
    g_battle_effect_model_command_word_offset = 0;
    return resource->objects[0].primitive_count;
}
