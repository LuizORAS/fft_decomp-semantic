/* SCUS_942.21 0x8001a9f4..0x8001aa43. */
#include "psx/libspu.h"

void SpuGetReverbModeParam(SpuReverbAttr* attr) {
    /* Volatile output stores keep each global read paired with its original store. */
    volatile SpuReverbAttr* output = attr;
    output->mode = _spu_rev_attr_mode;
    output->delay = _spu_rev_attr_delay;
    output->feedback = _spu_rev_attr_feedback;
    output->depth.left = _spu_rev_attr_depth_left;
    attr->depth.right = _spu_rev_attr_depth_right;
}
