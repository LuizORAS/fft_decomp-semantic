/* SCUS_942.21 0x8001b828..0x8001b8af. */
#include "psx/libspu.h"

/* Set voice `v_num`'s release rate; the release turns linear (the mode bit is cleared with the rate).
 * main_sound_quit sets the release when it silences the voices. */
void SpuSetVoiceRR(s32 v_num, u16 rr) {
    u16 value;
    value = _spu_RXX->raw[(v_num << 3) + 5];
    value &= 0xffc0;
    value |= rr;
    _spu_RXX->raw[(v_num << 3) + 5] = value;
    PSYQ_SPU_REGISTER_DELAY();
}
