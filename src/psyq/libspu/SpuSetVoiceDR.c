/* SCUS_942.21 0x8001b79c..0x8001b827. */
#include "psx/libspu.h"

void SpuSetVoiceDR(s32 v_num, u16 dr) {
    volatile psyq_spu_voice_t* voice = &_spu_RXX->voices[v_num];
    voice->adsr1 = (voice->adsr1 & 0xff0f) | (dr << 4);
    PSYQ_SPU_REGISTER_DELAY();
}
