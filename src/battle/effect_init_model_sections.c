#include "fft/battle_effect.h"
#include "psx/types.h"

/* Select the single object's relative sections and rewind its primitive stream. */
s32 effect_init_model_sections(psyq_tmd_header_t* resource, s32 unused_1) {
    g_effect_model_object = resource->objects;
    /* TMD offsets are byte counts from the object table; word indexing keeps signed rounding. */
    g_effect_model_vertices = (SVECTOR*)&((u32*)resource->objects)[(s32)resource->objects[0].vertex_offset / 4];
    g_effect_model_normals = (SVECTOR*)&((u32*)resource->objects)[(s32)resource->objects[0].normal_offset / 4];
    g_effect_model_commands
        = (psyq_tmd_packet_t*)&((u32*)resource->objects)[(s32)resource->objects[0].primitive_offset / 4];
    g_effect_model_command_word_offset = 0;
    return resource->objects[0].primitive_count;
}
