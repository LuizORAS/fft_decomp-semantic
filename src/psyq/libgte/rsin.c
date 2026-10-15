#include "psx/libgte.h"

/* The sine of `angle` (4096 a turn) in 1.3.12, so ONE is 1.0: sin_1 of the angle masked to 0-4095, negated for
 * a negative angle. About 190 functions (effects, the camera, the map, units) use it. */
s32 rsin(s32 angle) {
    s32 result;
    if (angle >= 0)
        result = sin_1(angle & 0xfff);
    else
        result = -sin_1(-angle & 0xfff);
    return result;
}
