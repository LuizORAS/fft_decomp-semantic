/* SCUS_942.21 0x8001b4b0..0x8001b627. */
#include "psx/libspu.h"

void SpuSetVoiceVolumeAttr(s32 v_num, s16 vol_l, s16 vol_r, s16 vol_mode_l, s16 vol_mode_r) {
    u32 left_mode;
    u32 right_mode;
    u16 right_volume;
    s32 register_index = v_num << 3;
    vol_l &= 0x7fff;
    left_mode = 0;
    switch (vol_mode_l) {
    case 1:
        left_mode = 0x8000;
        break;
    case 2:
        left_mode = 0x9000;
        break;
    case 3:
        left_mode = 0xa000;
        break;
    case 4:
        left_mode = 0xb000;
        break;
    case 5:
        left_mode = 0xc000;
        break;
    case 6:
        left_mode = 0xd000;
        break;
    case 7:
        left_mode = 0xe000;
        break;
    }
    right_volume = vol_r & 0x7fff;
    right_mode = 0;
    ((volatile u16*)_spu_RXX)[register_index] = vol_l | left_mode;
    switch (vol_mode_r) {
    case 1:
        right_mode = 0x8000;
        break;
    case 2:
        right_mode = 0x9000;
        break;
    case 3:
        right_mode = 0xa000;
        break;
    case 4:
        right_mode = 0xb000;
        break;
    case 5:
        right_mode = 0xc000;
        break;
    case 6:
        right_mode = 0xd000;
        break;
    case 7:
        right_mode = 0xe000;
        break;
    }
    ((volatile u16*)_spu_RXX)[register_index + 1] = right_volume | right_mode;
    PSYQ_SPU_REGISTER_DELAY();
}
