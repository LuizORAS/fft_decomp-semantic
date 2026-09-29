/* SCUS_942.21 0x8001b828..0x8001b8af. */
#include "psx/libspu.h"

void SpuSetVoiceRR(s32 v_num, u16 rr) {
    volatile psyq_spu_voice_t* voice = &_spu_RXX->voices[v_num];
    u16 value = voice->adsr2;
    value &= 0xffc0;
    value |= rr;
    voice->adsr2 = value;
    PSYQ_SPU_REGISTER_DELAY();
}
