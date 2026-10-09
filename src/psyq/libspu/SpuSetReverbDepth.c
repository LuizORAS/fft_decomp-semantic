/* SCUS_942.21 0x8001aa44..0x8001aabb. */
#include "psx/libspu.h"

s32 SpuSetReverbDepth(SpuReverbAttr* attr) {
    u32 mask = attr->mask;
    s32 all = mask == 0;
    if (all || (mask & 2)) {
        _spu_RXX->rxx.reverb_depth_left = attr->depth.left;
        _spu_rev_attr.depth.left = attr->depth.left;
    }
    if (all || (mask & 4)) {
        _spu_RXX->rxx.reverb_depth_right = attr->depth.right;
        _spu_rev_attr.depth.right = attr->depth.right;
    }
    return 0;
}
