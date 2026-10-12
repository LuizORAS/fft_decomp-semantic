#include "fft/effect.h"

/* Adds the placement offset to a point in the 9x17 dome lattice.
 * The stored coordinates retain the low 16 bits of each sum. */
void effect_dome_mesh_load_translated_vertex(s32 group, s32 point, const VECTOR* offset, SVECTOR* output) {
    int point_element = point * DOME_MESH_LATTICE_COMPONENTS;
    int element = group * ((DOME_MESH_SEGMENTS + 1) * DOME_MESH_LATTICE_COMPONENTS) + point_element;
    /* Typed point indexing caches the scratchpad base and changes the bytes. */
    u16* scratchpad = (u16*)0x1f800000;

    output->vx = scratchpad[element] + offset->vx;
    output->vy = scratchpad[element + 1] + offset->vy;
    output->vz = scratchpad[element + 2] + offset->vz;
}
