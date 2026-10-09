/* SCUS_942.21 0x8001bab8..0x8001bb5b. */
#include "psx/libspu.h"

void SpuSetVoiceRRAttr(s32 v_num, u16 rr, s32 rr_mode) {
    u32 mode = 0;
    u16 value;
    if (rr_mode != 3) {
        mode = rr_mode == 7 ? 0x20 : 0;
    }
    value = _spu_RXX->raw[(v_num << 3) + 5];
    value &= 0xffc0;
    value |= rr | mode;
    _spu_RXX->raw[(v_num << 3) + 5] = value;
    PSYQ_SPU_REGISTER_DELAY();
}
