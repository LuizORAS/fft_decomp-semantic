#include "fft/menu.h"

/* Interpolate with a 20.12 factor; ONE reaches end. Division truncates toward zero. */
s32 menu_lerp_fixed12(s32 start, s32 end, s32 factor) {
    return ((end - start) * factor) / ONE + start;
}
