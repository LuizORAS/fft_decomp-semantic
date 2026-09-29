/* SCUS_942.21 0x800192b0..0x800192eb. */
#include "psx/libspu.h"

u32 _spu_FgetRXXa(s32 reg_index, s32 raw) {
    u16 value = ((volatile u16*)_spu_RXX)[reg_index];
    if (raw == -1) {
        return value;
    }
    return value << _spu_mem_mode_plus;
}
