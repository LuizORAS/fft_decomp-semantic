/* SCUS_942.21 0x8001af64..0x8001afc3. */
#include "psx/libspu.h"

/* Send `size` bytes (at most 0x7eff0) from `addr` to SPU RAM at the transfer address, by DMA or programmed
 * I/O (_spu_Fw), and return the size. A DMA write ends in the background (_spu_FiDMA).
 * main_sound_transfer_spu_data calls it. */
u32 SpuWrite(u8* addr, u32 size) {
    if (size > 0x7eff0) {
        size = 0x7eff0;
    }
    _spu_Fw(addr, size);
    if (_spu_transferCallback == 0) {
        _spu_inTransfer = 0;
    }
    return size;
}
