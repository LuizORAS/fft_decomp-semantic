#include "psx/libgte.h"

/* The core of csqrt: a hyperbolic CORDIC iteration over the normalized `value`, with the shift-4 step
 * repeated, as that iteration needs to converge. */
s32 psyq_gte_csqrt_kernel(s32 value) {
    s32 x[8];
    s32 y[8];
    s32 shift;
    x[1] = value + 0x005d50ad;
    y[1] = value - 0x005d50ad;
    for (shift = 1; shift < 7; shift++) {
        if (shift != 4) {
            if (y[shift] >= 0) {
                x[shift + 1] = x[shift] - (y[shift] >> shift);
                y[shift + 1] = y[shift] - (x[shift] >> shift);
            } else {
                x[shift + 1] = x[shift] + (y[shift] >> shift);
                y[shift + 1] = y[shift] + (x[shift] >> shift);
            }
        } else {
            if (y[shift] >= 0) {
                s32 old_x = x[shift];
                s32 quarter;
                quarter = old_x >> 4;
                x[shift] -= y[shift] >> 4;
                y[shift] -= quarter;
                if (y[shift] >= 0) {
                    x[shift + 1] = x[shift] - (y[shift] >> 4);
                    y[shift + 1] = y[shift] - (x[shift] >> 4);
                } else {
                    old_x = y[shift];
                    x[shift + 1] = x[shift] + (old_x >> 4);
                    y[shift + 1] = y[shift] + (x[shift] >> 4);
                }
            } else {
                s32 old_x = x[shift];
                s32 quarter;
                quarter = old_x >> 4;
                x[shift] += y[shift] >> 4;
                y[shift] += quarter;
                if (y[shift] >= 0) {
                    x[shift + 1] = x[shift] - (y[shift] >> 4);
                    y[shift + 1] = y[shift] - (x[shift] >> 4);
                } else {
                    old_x = y[shift];
                    x[shift + 1] = x[shift] + (old_x >> 4);
                    y[shift + 1] = y[shift] + (x[shift] >> 4);
                }
            }
        }
    }
    return x[7];
}
