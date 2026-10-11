#include "psx/types.h"

/* Map a script music volume of 0..0x60 onto the driver's 0..0x7F; values of
 * 0x60 and above give full volume. */
s32 world_script_scale_music_volume(s32 value) {
    if (value == 0) {
        return 0;
    }
    if (value >= 0x60) {
        value = 0x7F;
    } else {
        value = (value * 0x7F) / 96;
    }
    return value;
}
