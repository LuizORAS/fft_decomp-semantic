#include "fft/main.h"

s32 main_sound_get_type(void) {
    s32 result;

    if (g_main_sound_driver_flags
        & (MAIN_SOUND_DRIVER_WIDE_OPPOSITE | MAIN_SOUND_DRIVER_WIDE | MAIN_SOUND_DRIVER_STEREO)) {
        result = MAIN_SOUND_TYPE_STEREO;
        if (g_main_sound_driver_flags & (MAIN_SOUND_DRIVER_WIDE_OPPOSITE | MAIN_SOUND_DRIVER_WIDE)) {
            result = MAIN_SOUND_TYPE_WIDE;
        }
    } else {
        result = MAIN_SOUND_TYPE_MONO;
    }
    return result;
}
