/* SCUS_942.21 0x8001b938..0x8001b9d3. */
#include "psx/libspu.h"

void SpuSetVoiceARAttr(s32 v_num, u16 ar, s32 ar_mode) {
    u32 mode = ar_mode == 5 ? 0x80 : 0;
    u16 value;
    value = _spu_RXX->raw[(v_num << 3) + 4];
    value &= 0xff;
    value |= (ar | mode) << 8;
    _spu_RXX->raw[(v_num << 3) + 4] = value;
    PSYQ_SPU_REGISTER_DELAY();
}
