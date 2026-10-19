#include "fft/menu.h"

/* Linear interpolation between start and end; factor is 20.12 fixed point (ONE gives end), and the
 * division truncates toward zero. WORLD compiles this text too. */
s32 menu_lerp_fixed12(s32 start, s32 end, s32 factor) {
    return ((end - start) * factor) / ONE + start;
}
