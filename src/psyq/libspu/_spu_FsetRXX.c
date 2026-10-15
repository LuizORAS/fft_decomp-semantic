/* SCUS_942.21 0x800191c4..0x8001920b. */
#include "psx/libspu.h"

/* Write SPU register `reg_index` (counted in halfwords): `value` as it is, or a byte address in eight-byte
 * units when `is_address` is set. _SpuInit and SpuSetReverbModeParam set the reverb work area start with it. */
void _spu_FsetRXX(s32 reg_index, u32 value, s32 is_address) {
    if (is_address == PSYQ_SPU_REGISTER_RAW) {
        _spu_RXX->raw[reg_index] = value;
    } else {
        _spu_RXX->raw[reg_index] = value >> _spu_mem_mode_plus;
    }
}
