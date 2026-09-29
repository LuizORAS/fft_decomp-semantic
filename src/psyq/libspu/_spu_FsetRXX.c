/* SCUS_942.21 0x800191c4..0x8001920b. */
#include "psx/libspu.h"

void _spu_FsetRXX(s32 reg_index, u32 value, s32 is_address) {
    if (is_address == PSYQ_SPU_REGISTER_RAW) {
        ((volatile u16*)_spu_RXX)[reg_index] = value;
    } else {
        ((volatile u16*)_spu_RXX)[reg_index] = value >> _spu_mem_mode_plus;
    }
}
