/* SCUS_942.21 0x8001920c..0x800192af. */
#include "psx/libspu.h"

u32 _spu_FsetRXXa(s32 reg_index, u32 address) {
    u32 units;
    if (_spu_mem_mode && address % _spu_mem_mode_unit) {
        address += _spu_mem_mode_unit;
        address &= ~_spu_mem_mode_unitM;
    }
    units = address >> _spu_mem_mode_plus;
    switch (reg_index) {
    case PSYQ_SPU_ADDRESS_AS_UNITS:
        return units & 0xffff;
    case PSYQ_SPU_ADDRESS_ALIGNED_BYTES:
        return address;
    default:
        ((volatile u16*)_spu_RXX)[reg_index] = units;
        return address;
    }
}
