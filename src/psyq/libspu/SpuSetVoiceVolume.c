/* SCUS_942.21 0x8001b428..0x8001b4af. */
#include "psx/libspu.h"

void SpuSetVoiceVolume(s32 v_num, s16 vol_l, s16 vol_r) {
    volatile psyq_spu_voice_t* voice;
    vol_l &= 0x7fff;
    vol_r &= 0x7fff;
    voice = &_spu_RXX->voices[v_num];
    voice->volume_left = vol_l;
    voice->volume_right = vol_r;
    PSYQ_SPU_REGISTER_DELAY();
}
