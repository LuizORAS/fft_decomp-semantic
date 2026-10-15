#include "fft/main.h"
#include "psx/types.h"

/* SPU transfer-complete callback: clears the driver's read, write and
 * transfer-busy flags. */
void main_smd_spu_transfer_callback(void) {
    g_main_sound_driver_flags
        &= ~(MAIN_SOUND_DRIVER_SPU_READ | MAIN_SOUND_DRIVER_SPU_WRITE | MAIN_SOUND_DRIVER_SPU_TRANSFER);
}
