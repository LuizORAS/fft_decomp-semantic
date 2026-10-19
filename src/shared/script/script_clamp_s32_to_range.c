#include "fft/script.h"

/* Clamp `*value` into min..max in place; ChangeStats keeps HP within the unit's maximum and
 * Brave, Faith and CT within 0-100 with it. BATTLE and WORLD compile this text. */
void script_clamp_s32_to_range(s32* value, s32 min, s32 max) {
    if (*value < min) {
        *value = min;
        return;
    }
    if (*value > max) {
        *value = max;
    }
}
