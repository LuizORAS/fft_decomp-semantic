/* SCUS_942.21 0x8001b828..0x8001b8af. */
#include "psx/libspu.h"

/* Set voice `v_num`'s release rate; the release turns linear (the mode bit is cleared with the rate).
 * suzuki_sound_quit sets the release when it silences the voices. */
void SpuSetVoiceRR(s32 v_num, u16 rr) {
    /* Adding the index before the register base preserves the retail address operands. */
    u16 value;
    value = (v_num + _spu_RXX->rxx.voices)->adsr2;
    value &= 0xffc0;
    value |= rr;
    (v_num + _spu_RXX->rxx.voices)->adsr2 = value;
    PSYQ_SPU_REGISTER_DELAY();
}
