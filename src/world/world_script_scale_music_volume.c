#include "psx/types.h"

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
