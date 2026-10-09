#include "fft/main.h"
#include "psx/types.h"

void main_suzuki_spu_callback_func(void) {
    g_main_sound_driver_flags
        &= ~(MAIN_SOUND_DRIVER_SPU_READ | MAIN_SOUND_DRIVER_SPU_WRITE | MAIN_SOUND_DRIVER_SPU_TRANSFER);
}
