/* SCUS_942.21 0x8001b9d4..0x8001bab7. */
#include "psx/libspu.h"

void SpuSetVoiceSRAttr(s32 v_num, u16 sr, s32 sr_mode) {
    volatile psyq_spu_voice_t* voice;
    /* The unpinned halfword index swaps v1 and a0 in the retail register allocation. */
    register s32 register_index __asm__("$3") = v_num << 3;
    u32 mode = 0x100;
    switch (sr_mode) {
    case 1:
        mode = 0;
        break;
    case 5:
        mode = 0x200;
        break;
    case 7:
        mode = 0x300;
        break;
    }
    /* A halfword index preserves the retail split address calculation. */
    voice = (psyq_spu_voice_t*)((u16*)_spu_RXX + register_index);
    voice->adsr2 = (voice->adsr2 & 0x3f) | ((sr | mode) << 6);
    PSYQ_SPU_REGISTER_DELAY();
}
