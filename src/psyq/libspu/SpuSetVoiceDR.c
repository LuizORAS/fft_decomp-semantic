/* SCUS_942.21 0x8001b79c..0x8001b827. */
#include "psx/libspu.h"

void SpuSetVoiceDR(s32 v_num, u16 dr) {
    _spu_RXX->raw[(v_num << 3) + 4] = (_spu_RXX->raw[(v_num << 3) + 4] & 0xff0f) | (dr << 4);
    PSYQ_SPU_REGISTER_DELAY();
}
