/* SCUS_942.21 0x8001ac7c..0x8001acef. */
#include "psx/libspu.h"

/* Read the SPU's capture buffers of decoded CD audio and voices 1 and 3 into `d_data` by DMA: flag 5 reads
 * the two CD buffers, 6 the two voice buffers, anything else all four (4 KB). Returns 1 when the SPU is
 * writing the second half of each buffer (psx-spx: status bit 11). suzuki_sound_transfer_spu_data reads all four
 * (0) or the CD buffers (5). */
s32 SpuReadDecodedData(SpuDecodedData* d_data, s32 flag) {
    s16* destination = d_data->cd_left;
    u16 address;
    u32 blocks;
    u32 status;
    switch (flag) {
    case SPU_CDONLY:
        address = 0;
        blocks = 0x20;
        break;
    case SPU_VOICEONLY:
        destination = d_data->voice1;
        address = 0x100;
        blocks = 0x20;
        break;
    default:
        address = 0;
        blocks = 0x40;
        break;
    }
    _spu_Fr_(destination, address, blocks);
    status = _spu_RXX->rxx.status & 0x800;
    return status != 0;
}
