/* SCUS_942.21 0x80019d88..0x80019dd7. */
#include "psx/libspu.h"

s32 SpuSetNoiseClock(s32 n_clock) {
    /* Without this input pin GCC removes the retail v0-to-a1 copy. */
    register s32 requested __asm__("$2") = n_clock;
    s32 clock = requested;
    if (requested < 0) {
        clock = 0;
    } else if (clock >= 64) {
        clock = 63;
    }
    {
        /* Unpinned bits move the return copy before the register update. */
        register u32 bits __asm__("$2") = (clock & 0x3f) << 8;
        u32 control = _spu_RXX->control;
        control &= 0xc0ff;
        control |= bits;
        _spu_RXX->control = control;
    }
    return clock;
}
