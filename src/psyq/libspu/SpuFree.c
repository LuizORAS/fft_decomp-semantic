/* SCUS_942.21 0x80019ae0..0x80019b5b. */
#include "psx/libspu.h"

/* Free the SPU RAM block at `addr` and merge the free records (_spu_gcSPU). main_sound_free_spu_ram calls it. */
void SpuFree(u32 addr) {
    s32 index = 0;
    psyq_spu_heap_record_t* record;
    for (; index < _spu_AllocBlockNum; index++) {
        record = &_spu_memList[index];
        if (record->address & PSYQ_SPU_HEAP_TAIL) {
            break;
        }
        if (record->address == addr) {
            record->address = addr | PSYQ_SPU_HEAP_FREE;
            break;
        }
    }
    _spu_gcSPU();
}
