/* SCUS_942.21 0x8001ac7c..0x8001acef. */
#include "psx/libspu.h"

s32 SpuReadDecodedData(SpuDecodedData* d_data, s32 flag) {
    u16 address;
    u32 blocks;
    u32 status;
    switch (flag) {
    case 5:
        address = 0;
        blocks = 0x20;
        break;
    case 6:
        d_data = (SpuDecodedData*)d_data->voice1;
        address = 0x100;
        blocks = 0x20;
        break;
    default:
        address = 0;
        blocks = 0x40;
        break;
    }
    _spu_Fr_(d_data, address, blocks);
    status = _spu_RXX->status & 0x800;
    /* A natural narrow-field test becomes srl/andi instead of retail andi/sltu. */
    __asm__("" : "=r"(status) : "0"(status));
    return status != 0;
}
