/* SCUS_942.21 0x80019d88..0x80019dd7. */
#include "psx/libspu.h"

/* Set the noise generator's clock to `n_clock`, clamped to 0-63, and return it. main_smd_enable_noise and
 * main_smd_increase_noise call it. */
s32 SpuSetNoiseClock(s32 n_clock) {
    s32 clock;
    if (n_clock < 0) {
        clock = 0;
    } else if (n_clock >= 64) {
        clock = 63;
    } else {
        clock = n_clock;
    }
    _spu_RXX->rxx.control = (_spu_RXX->rxx.control & 0xc0ff) | ((clock & 0x3f) << 8);
    return clock;
}
