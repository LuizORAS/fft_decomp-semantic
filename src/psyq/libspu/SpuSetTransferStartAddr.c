/* SCUS_942.21 0x8001afc4..0x8001b017. */
#include "psx/libspu.h"

/* Set the SPU RAM address of the next transfer, rounded up to 8 bytes; returns it in eight-byte units, or
 * 0 for an address outside 0x1010-0x7fff8. main_sound_transfer_spu_data calls it. */
u32 SpuSetTransferStartAddr(u32 addr) {
    if (addr - 0x1010 > 0x7efe8) {
        return 0;
    }
    _spu_tsa = _spu_FsetRXXa(PSYQ_SPU_ADDRESS_AS_UNITS, addr);
    return _spu_tsa;
}
