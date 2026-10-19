#include "psx/libgte.h"

/* The angle of the vector (`x`, `y`) from -2048 to 2048 (4096 a turn; 0 along +x, 1024 along +y), from
 * LIBGTE's arctangent table indexed by 1024 times the ratio of the smaller component to the larger; large
 * components are scaled down before the division. 0 for (0, 0). Effects, the camera and units turn towards
 * their targets with it. */
long ratan2(long y, long x) {
    long negative_x = 0;
    long negative_y = 0;
    long result;
    if (x < 0) {
        negative_x = 1;
        x = -x;
    }
    if (y < 0) {
        negative_y = 1;
        y = -y;
    }
    if (!x && !y)
        return 0;
    if (y < x) {
        if (y & 0x7fe00000)
            y /= x >> 10;
        else
            y = (y << 10) / x;
        result = g_psyq_gte_atan_ratio_table[y];
    } else {
        if (x & 0x7fe00000)
            y = x / (y >> 10);
        else
            y = (x << 10) / y;
        result = ONE / 4 - g_psyq_gte_atan_ratio_table[y];
    }
    if (negative_x)
        result = ONE / 2 - result;
    if (negative_y)
        result = -result;
    return result;
}
