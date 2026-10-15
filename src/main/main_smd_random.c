#include "fft/main.h"
#include "psx/types.h"

/* Step the driver's xorshift generator (state ^= state << 17, then state ^= state >> 15) and return the
 * low 15 bits of the new state. The two random modulator waveforms use it; SuzukiSPUInitialiser seeds it
 * with 0x12345678. */
s32 main_smd_random(void) {
    s32 state;

    state = g_main_smd_random_state;
    state ^= state << 17;
    state ^= state >> 15;
    g_main_smd_random_state = state;
    return state & 0x7fff;
}
