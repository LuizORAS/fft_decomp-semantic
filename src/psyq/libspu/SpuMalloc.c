/* SCUS_942.21 0x80019518..0x800197df. */
#include "psx/libspu.h"

s32 SpuMalloc(s32 size) {
    /* The remaining pins preserve retail register allocation for heap scans and record updates. */
    s32 requested = size;
    s32 index = 0;
    s32 selected = -1;
    u32 reserved;
    s32 rounded;
    if (_spu_rev_reserve_wa == 0) {
        reserved = 0;
    } else {
        reserved = (0x10000 - _spu_rev_offsetaddr) << _spu_mem_mode_plus;
    }
    rounded = requested;
    if (requested & ~_spu_mem_mode_unitM) {
        rounded = requested + _spu_mem_mode_unitM;
    }
    requested = rounded >> _spu_mem_mode_plus;
    requested <<= _spu_mem_mode_plus;
    if (_spu_memList[0].address & PSYQ_SPU_HEAP_TAIL) {
        selected = 0;
    } else {
        register s32 capacity __asm__("$3");
        register u32 offset __asm__("$2");
        _spu_gcSPU();
        capacity = _spu_AllocBlockNum;
        if (index < capacity) {
            register u32 tail_flag __asm__("$7") = PSYQ_SPU_HEAP_TAIL;
            register u32 free_flag __asm__("$6") = PSYQ_SPU_HEAP_FREE;
            s32 limit = capacity;
            register psyq_spu_heap_record_t* table __asm__("$3");
            psyq_spu_heap_record_t* scan;
            offset = index * sizeof(psyq_spu_heap_record_t);
            /* This tie keeps both flag immediates before the table-pointer load. */
            __asm__("" : : "r"(tail_flag), "r"(free_flag));
            table = _spu_memList;
            /* Integer address addition retains the original operand order. */
            scan = (psyq_spu_heap_record_t*)(offset + (u32)table);
            for (; index < limit; index++, scan++) {
                if ((scan->address & PSYQ_SPU_HEAP_TAIL)
                    || ((scan->address & PSYQ_SPU_HEAP_FREE) && scan->size >= (u32)requested)) {
                    selected = index;
                    break;
                }
            }
        }
    }
    index = selected * sizeof(psyq_spu_heap_record_t);
    if (selected == -1) {
        return -1;
    }
    {
        register psyq_spu_heap_record_t* table __asm__("$7") = _spu_memList;
        psyq_spu_heap_record_t* record = (psyq_spu_heap_record_t*)(index + (u32)table);
        register u32 address __asm__("$3");
        address = record->address;
        /* Volatile ordering keeps the tail-flag immediate after this address load. */
        __asm__ volatile("" : "=r"(address) : "0"(address));
        if (address & PSYQ_SPU_HEAP_TAIL) {
            s32 new_tail;
            psyq_spu_heap_record_t* next;
            u32 current_address;
            if (selected >= _spu_AllocBlockNum)
                return -1;
            if (record->size - reserved < (u32)requested)
                return -1;
            {
                u32 mask = PSYQ_SPU_HEAP_ADDRESS_MASK;
                new_tail = selected + 1;
                /* Without the tie GCC destructively increments the selected record index. */
                __asm__("" : "=r"(new_tail) : "0"(new_tail));
                current_address = record->address;
                next = (psyq_spu_heap_record_t*)((u32)(new_tail * sizeof(*next)) + (u32)table);
                next->address = ((current_address & mask) + requested) | PSYQ_SPU_HEAP_TAIL;
                next->size = record->size - requested;
                _spu_AllocLastNum = new_tail;
                record->size = requested;
                record->address &= mask;
                _spu_gcSPU();
                {
                    psyq_spu_heap_record_t* returned = (psyq_spu_heap_record_t*)(index + (u32)_spu_memList);
                    return returned->address;
                }
            }
        }
        {
            u32 free_size = record->size;
            /* Retail recomputes the byte offset in this branch's delay slot. */
            __asm__("" : "=r"(selected) : "0"(selected), "r"(free_size));
            index = selected * sizeof(*record);
            if ((u32)requested < free_size) {
                s32 old_tail = _spu_AllocLastNum;
                if (old_tail < _spu_AllocBlockNum) {
                    register psyq_spu_heap_record_t* next __asm__("$2");
                    u32 old_size;
                    register u32 old_address __asm__("$5");
                    register u32 value __asm__("$3");
                    address = requested + address;
                    next = (psyq_spu_heap_record_t*)((u32)(old_tail * sizeof(*next)) + (u32)table);
                    address |= PSYQ_SPU_HEAP_FREE;
                    old_address = next->address;
                    old_size = next->size;
                    /* These ties retain the original saved-field registers and store order. */
                    __asm__("" : "=r"(old_address), "=r"(old_size) : "0"(old_address), "1"(old_size));
                    next->address = address;
                    value = free_size - requested;
                    next->size = value;
                    value = old_tail + 1;
                    _spu_AllocLastNum = value;
                    next[1].address = old_address;
                    next[1].size = old_size;
                }
            }
        }
    }
    {
        u32 address_mask = 0x0fff0000;
        register psyq_spu_heap_record_t* record __asm__("$3");
        u32 address;
        /* Splitting and tying the mask retains its original load-delay scheduling. */
        __asm__("" : "=r"(address_mask) : "0"(address_mask));
        record = (psyq_spu_heap_record_t*)(index + (u32)_spu_memList);
        /* The tie retains v1 for the final allocation record. */
        __asm__("" : "=r"(record) : "0"(record));
        address = record->address;
        address_mask |= 0xffff;
        record->size = requested;
        address &= address_mask;
        record->address = address;
        _spu_gcSPU();
        index += (u32)_spu_memList;
        return ((psyq_spu_heap_record_t*)index)->address;
    }
}
