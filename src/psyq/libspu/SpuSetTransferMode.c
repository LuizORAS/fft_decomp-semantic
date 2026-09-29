/* SCUS_942.21 0x8001b018..0x8001b04b. */
#include "psx/libspu.h"

s32 SpuSetTransferMode(s32 mode) {
    s32 normalized;
    switch (mode) {
    case 0:
        normalized = 0;
        break;
    case 1:
        normalized = 1;
        break;
    default:
        normalized = 0;
        break;
    }
    _spu_trans_mode = mode;
    _spu_transMode = normalized;
    return normalized;
}
