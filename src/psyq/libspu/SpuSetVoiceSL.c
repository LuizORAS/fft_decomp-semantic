/* SCUS_942.21 0x8001b8b0..0x8001b937. */
#include "psx/libspu.h"

void SpuSetVoiceSL(s32 v_num, u16 sl) {
    volatile psyq_spu_voice_t* voice = &_spu_RXX->voices[v_num];
    u16 value = voice->adsr1;
    value &= 0xfff0;
    value |= sl;
    voice->adsr1 = value;
    PSYQ_SPU_REGISTER_DELAY();
}
