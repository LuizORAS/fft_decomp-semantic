#include "fft/script.h"

/* Clamp a signed value, testing the lower bound first. */
void script_clamp_s32_to_range(s32* value, s32 min, s32 max) {
    if (*value < min) {
        *value = min;
        return;
    }
    if (*value > max) {
        *value = max;
    }
}
