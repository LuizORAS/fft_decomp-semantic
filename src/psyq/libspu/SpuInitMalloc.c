/* SCUS_942.21 0x800194c4..0x80019517. */
#include "psx/libspu.h"

s32 SpuInitMalloc(s32 num, u8* top) {
    /* This pin retains the original leading move from a0 into the returned v0. */
    register s32 count __asm__("$2") = num;
    psyq_spu_heap_record_t* table = (psyq_spu_heap_record_t*)top;
    if (count <= 0) {
        count = 0;
    } else {
        table[0].address = (PSYQ_SPU_HEAP_TAIL | PSYQ_SPU_HEAP_START);
        _spu_memList = table;
        _spu_AllocLastNum = 0;
        _spu_AllocBlockNum = count;
        table[0].size = (0x10000 << _spu_mem_mode_plus) - PSYQ_SPU_HEAP_START;
    }
    return count;
}
