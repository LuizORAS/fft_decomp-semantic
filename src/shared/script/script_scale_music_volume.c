#include "fft/script.h"

/* Convert a script music volume (0..0x60) to the sound driver's 0..0x7f range: 0
 * stays 0, 0x60 and above give 0x7f, and the values between scale by 127 / 96. */
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
