/* SCUS_942.21 0x8001b628..0x8001b6a3. */
#include "psx/libspu.h"

void SpuSetVoicePitch(s32 v_num, u16 pitch) {
    _spu_RXX->raw[(v_num << 3) + 2] = pitch;
    PSYQ_SPU_REGISTER_DELAY();
}
