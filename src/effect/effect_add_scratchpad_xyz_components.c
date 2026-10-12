#include "fft/effect.h"

/* Adds the placement offset to a point in the 9x17 dome lattice.
 * The stored coordinates retain the low 16 bits of each sum. */
void effect_add_scratchpad_xyz_components(s32 group, s32 point, const VECTOR* offset, SVECTOR* output) {
    int point_element = point * 3;
    int element = group * (17 * 3) + point_element;
    /* Typed point indexing caches the scratchpad base and changes the bytes. */
    u16* scratchpad = (u16*)0x1f800000;

    output->vx = scratchpad[element] + offset->vx;
    output->vy = scratchpad[element + 1] + offset->vy;
    output->vz = scratchpad[element + 2] + offset->vz;
}
