#include "fft/world.h"
#include "psx/types.h"

/* Linear interpolation in 20.12 fixed point:
 * start + (end - start) * fraction / value, as a 20.12 result. */
s32 world_script_interpolate_range_fixed12(s32 start, s32 end, s32 fraction, s32 value) {
    return world_util_mul_div_s64(end - start, fraction << 12, value) + (start << 12);
}
