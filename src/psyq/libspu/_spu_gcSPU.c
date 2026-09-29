/* SCUS_942.21 0x800197e0..0x80019adf. */
#include "psx/libspu.h"

void _spu_gcSPU(void) {
    s32 index;
    s32 successor;
    for (index = 0; index <= _spu_AllocLastNum;) {
        if (_spu_memList[index].address & PSYQ_SPU_HEAP_FREE) {
            psyq_spu_heap_record_t* scan;
            successor = index + 1;
            scan = &_spu_memList[successor];
            while (1) {
                if ((scan++)->address != PSYQ_SPU_HEAP_DELETED)
                    break;
                successor++;
            }
            if ((_spu_memList[successor].address & PSYQ_SPU_HEAP_FREE)
                && (_spu_memList[successor].address & PSYQ_SPU_HEAP_ADDRESS_MASK)
                    == (_spu_memList[index].address & PSYQ_SPU_HEAP_ADDRESS_MASK) + _spu_memList[index].size) {
                _spu_memList[successor].address = PSYQ_SPU_HEAP_DELETED;
                _spu_memList[index].size += _spu_memList[successor].size;
                continue;
            }
        }
        index++;
    }
    for (index = 0; index <= _spu_AllocLastNum; index++) {
        if (_spu_memList[index].size == 0)
            _spu_memList[index].address = PSYQ_SPU_HEAP_DELETED;
    }
    for (index = 0; index <= _spu_AllocLastNum; index++) {
        if (_spu_memList[index].address & PSYQ_SPU_HEAP_TAIL)
            break;
        for (successor = index + 1; successor <= _spu_AllocLastNum; successor++) {
            if (_spu_memList[successor].address & PSYQ_SPU_HEAP_TAIL)
                break;
            if ((_spu_memList[successor].address & PSYQ_SPU_HEAP_ADDRESS_MASK)
                < (_spu_memList[index].address & PSYQ_SPU_HEAP_ADDRESS_MASK)) {
                u32 address = _spu_memList[index].address;
                u32 size;
                _spu_memList[index].address = _spu_memList[successor].address;
                size = _spu_memList[index].size;
                _spu_memList[index].size = _spu_memList[successor].size;
                _spu_memList[successor].address = address;
                _spu_memList[successor].size = size;
            }
        }
    }
    for (index = 0; index <= _spu_AllocLastNum; index++) {
        if (_spu_memList[index].address & PSYQ_SPU_HEAP_TAIL)
            break;
        if (_spu_memList[index].address == PSYQ_SPU_HEAP_DELETED) {
            _spu_memList[index].address = _spu_memList[_spu_AllocLastNum].address;
            _spu_memList[index].size = _spu_memList[_spu_AllocLastNum].size;
            _spu_AllocLastNum = index;
            break;
        }
    }
    for (index = _spu_AllocLastNum - 1; index >= 0; index--) {
        if (!(_spu_memList[index].address & PSYQ_SPU_HEAP_FREE))
            break;
        _spu_memList[index].address = (_spu_memList[index].address & PSYQ_SPU_HEAP_ADDRESS_MASK) | PSYQ_SPU_HEAP_TAIL;
        _spu_memList[index].size += _spu_memList[_spu_AllocLastNum].size;
        _spu_AllocLastNum = index;
    }
}
