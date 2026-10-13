#include "fft/script.h"

/* Map script volume 0..96 to driver volume 0..127; values above 96 saturate. */
s32 script_scale_music_volume(s32 volume) {
    if (volume == 0) {
        return 0;
    }
    if (volume >= 96) {
        volume = 127;
    } else {
        volume = (volume * 127) / 96;
    }
    return volume;
}
