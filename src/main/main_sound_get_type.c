#include "fft/main.h"

/* The sound output setting as MAIN_SOUND_TYPE_MONO, _STEREO or _WIDE, read from
 * the driver flags. The opposite-phase wide mode is reported as _WIDE. */
main_sound_type_e main_sound_get_type(void) {
    main_sound_type_e result;

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
