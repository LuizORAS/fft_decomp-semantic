/* SCUS_942.21 0x8001b628..0x8001b6a3. */
#include "psx/libspu.h"

void SpuSetVoicePitch(s32 v_num, u16 pitch) {
    volatile psyq_spu_voice_t* voice = &_spu_RXX->voices[v_num];
    voice->pitch = pitch;
    PSYQ_SPU_REGISTER_DELAY();
}
