/* SCUS_942.21 0x8001b938..0x8001b9d3. */
#include "psx/libspu.h"

void SpuSetVoiceARAttr(s32 v_num, u16 ar, s32 ar_mode) {
    volatile psyq_spu_voice_t* voice = &_spu_RXX->voices[v_num];
    u32 mode = ar_mode == 5 ? 0x80 : 0;
    voice->adsr1 = (voice->adsr1 & 0xff) | ((ar | mode) << 8);
    PSYQ_SPU_REGISTER_DELAY();
}
