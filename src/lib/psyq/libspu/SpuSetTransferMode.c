/* SCUS_942.21 0x8001b018..0x8001b04b. */
#include "psx/libspu.h"

/* Set how SpuWrite sends data: 1 by programmed I/O through the SPU's FIFO, anything else by DMA (0).
 * Returns that choice. suzuki_sound_transfer_spu_data sets DMA. */
s32 SpuSetTransferMode(s32 mode) {
    s32 normalized;
    switch (mode) {
    case SPU_TRANSFER_BY_DMA:
        normalized = SPU_TRANSFER_BY_DMA;
        break;
    case SPU_TRANSFER_BY_IO:
        normalized = SPU_TRANSFER_BY_IO;
        break;
    default:
        normalized = SPU_TRANSFER_BY_DMA;
        break;
    }
    _spu_trans_mode = mode;
    _spu_transMode = normalized;
    return normalized;
}
