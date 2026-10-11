/* SCUS_942.21 0x8001b8b0..0x8001b937. */
#include "psx/libspu.h"

void SpuSetVoiceSL(s32 v_num, u16 sl) {
    u16 value;
    value = _spu_RXX->raw[(v_num << 3) + 4];
    value &= 0xfff0;
    value |= sl;
    _spu_RXX->raw[(v_num << 3) + 4] = value;
    PSYQ_SPU_REGISTER_DELAY();
}
