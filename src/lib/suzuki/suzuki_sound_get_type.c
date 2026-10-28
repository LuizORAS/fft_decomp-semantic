#include "psx/suzuki.h"

/* The sound output setting as MAIN_SUZUKI_SOUND_TYPE_MONO, _STEREO or _WIDE, read from
 * the driver flags. The opposite-phase wide mode is reported as _WIDE. */
suzuki_sound_type_e suzuki_sound_get_type(void) {
    suzuki_sound_type_e result;

    if (g_suzuki_sound_driver_flags
        & (MAIN_SUZUKI_SOUND_DRIVER_WIDE_OPPOSITE | MAIN_SUZUKI_SOUND_DRIVER_WIDE | MAIN_SUZUKI_SOUND_DRIVER_STEREO)) {
        result = MAIN_SUZUKI_SOUND_TYPE_STEREO;
        if (g_suzuki_sound_driver_flags & (MAIN_SUZUKI_SOUND_DRIVER_WIDE_OPPOSITE | MAIN_SUZUKI_SOUND_DRIVER_WIDE)) {
            result = MAIN_SUZUKI_SOUND_TYPE_WIDE;
        }
    } else {
        result = MAIN_SUZUKI_SOUND_TYPE_MONO;
    }
    return result;
}
