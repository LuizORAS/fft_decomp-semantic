#include "psx/types.h"

/* Convert a script music volume (0..0x60) to the sound driver's 0..0x7f range: 0
 * stays 0, 0x60 and above give 0x7f, and the values between scale by 127 / 96. */
s32 battle_script_scale_music_volume(s32 volume) {
    if (volume == 0) {
        return 0;
    }
    if (volume >= 0x60) {
        volume = 0x7F;
    } else {
        volume = (volume * 0x7F) / 96;
    }
    return volume;
}
