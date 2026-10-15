/* SCUS_942.21 0x8001b428..0x8001b4af. */
#include "psx/libspu.h"

/* Set voice `v_num`'s left and right volumes (15 bits each, no sweep). */
void SpuSetVoiceVolume(s32 v_num, s16 vol_l, s16 vol_r) {
    vol_l &= 0x7fff;
    vol_r &= 0x7fff;
    _spu_RXX->raw[v_num << 3] = vol_l;
    _spu_RXX->raw[(v_num << 3) + 1] = vol_r;
    PSYQ_SPU_REGISTER_DELAY();
}
