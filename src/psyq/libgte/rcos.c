#include "psx/libgte.h"

/* The cosine of `angle` (4096 a turn) in 1.3.12, from the same quarter-wave table as sin_1. About 190
 * functions use it. */
s32 rcos(s32 angle) {
    s32 result;
    if (angle < 0)
        angle = -angle;
    angle &= 0xfff;
    if (angle <= 2048) {
        if (angle <= 1024)
            result = g_psyq_gte_sine_quarter_table[1024 - angle];
        else
            result = -g_psyq_gte_sine_quarter_table[angle - 1024];
    } else {
        if (angle > 3072)
            result = g_psyq_gte_sine_quarter_table[angle - 3072];
        else
            result = -g_psyq_gte_sine_quarter_table[3072 - angle];
    }
    return result;
}
