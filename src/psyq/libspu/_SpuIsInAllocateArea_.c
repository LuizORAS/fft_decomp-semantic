/* SCUS_942.21 0x80019f88..0x8001a013. */
#include "psx/libspu.h"

s32 _SpuIsInAllocateArea_(u32 address) {
    s32 index;
    u32 start;
    address <<= _spu_mem_mode_plus;
    if (_spu_memList == 0) {
        return 0;
    }
    for (index = 0;; index++) {
        start = _spu_memList[index].address;
        if (start & PSYQ_SPU_HEAP_FREE) {
            continue;
        }
        if (start & PSYQ_SPU_HEAP_TAIL) {
            break;
        }
        start &= PSYQ_SPU_HEAP_ADDRESS_MASK;
        if (start >= address) {
            return 1;
        }
        if (address < start + _spu_memList[index].size) {
            return 1;
        }
    }
    return 0;
}
