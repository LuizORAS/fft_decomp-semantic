#include "fft/main.h"
#include "psx/types.h"

/* Driver API setter left without a consumer: nothing on the disc calls it or
 * loads D_80032A28, which SuzukiSPUInitialiser and main_sound_quit zero. */
void main_sound_set_unread_value_800184e0(u16 value) {
    D_80032A28 = value;
}
