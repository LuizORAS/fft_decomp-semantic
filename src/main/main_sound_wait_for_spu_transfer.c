#include "fft/main.h"
#include "psx/types.h"

/* Volatile view: the target re-reads the transfer flag on every poll. */
extern volatile s16 g_main_sound_driver_flags;

s16 main_sound_wait_for_spu_transfer(s32 flags) {
    if (flags & MAIN_SOUND_SPU_TRANSFER_WAIT) {
        while (g_main_sound_driver_flags & MAIN_SOUND_DRIVER_SPU_TRANSFER) { }
    }
    if (g_main_sound_driver_flags & MAIN_SOUND_DRIVER_SPU_TRANSFER) {
        return g_main_sound_spu_transfer_status_records[g_main_sound_spu_transfer_status_index * 8];
    }
    return 0;
}
