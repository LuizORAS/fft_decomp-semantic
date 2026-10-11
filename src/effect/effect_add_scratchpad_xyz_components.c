#include "fft/effect.h"

/* Stores scratchpad xyz triple group * 17 + point plus offset into output.
 *
 * The stored coordinates retain the low halfwords of each sum. */
void effect_add_scratchpad_xyz_components(s32 group, s32 point, const VECTOR* offset, SVECTOR* output) {
    int point_offset = point * 3;
    int group_offset = group * 51;
    int element = group_offset + point_offset;
    u16* scratchpad = (u16*)0x1f800000;

    output->vx = scratchpad[element] + offset->vx;
    output->vy = scratchpad[element + 1] + offset->vy;
    output->vz = scratchpad[element + 2] + offset->vz;
}
