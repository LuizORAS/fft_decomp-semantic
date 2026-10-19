/* SCUS_942.21 0x8001915c..0x800191c3. */
#include "psx/libspu.h"

/* Read `size` bytes of SPU RAM at the transfer address into `addr` by DMA (_spu_t). Returns the size.
 * SpuRead calls it. */
u32 _spu_Fr(void* addr, u32 size) {
    _spu_t(PSYQ_SPU_DMA_SET_ADDRESS, _spu_tsa << _spu_mem_mode_plus);
    _spu_t(PSYQ_SPU_DMA_READ);
    _spu_t(PSYQ_SPU_DMA_START_TRANSFER, addr, size);
    return size;
}
