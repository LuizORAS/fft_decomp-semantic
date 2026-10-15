#include "fft/main.h"
#include "psx/types.h"

/* Set the state of main_smd_random. Nothing calls it: SuzukiSPUInitialiser writes its seed itself. */
void main_smd_seed_random(s32 value) {
    g_main_smd_random_state = value;
}
