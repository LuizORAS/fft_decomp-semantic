#include "psx/libgte.h"

/* main 0x8001be1c–0x8001beb8. */
long csqrt(long value) {
    long result;
    long difference;
    long scale;
    long normalized;
    if (value == 0)
        result = 0;
    else {
        difference = 8 - Lzc(value);
        scale = difference >> 1;
        if (difference >= 0)
            normalized = value >> (scale << 1);
        else {
            __asm__("" : "=r"(difference) : "0"(difference)); /* Retail recomputes the half exponent in this arm. */
            scale = (difference >> 1) + 1;
            normalized = value << -(scale << 1);
        }
        scale -= 6;
        if (scale < 0)
            result = psyq_gte_csqrt_kernel(normalized) >> -scale;
        else
            result = psyq_gte_csqrt_kernel(normalized) << scale;
    }
    return result;
}
