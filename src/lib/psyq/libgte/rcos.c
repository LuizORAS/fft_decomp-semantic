#include "psx/libgte.h"

/* The cosine of `angle` (4096 a turn) in 1.3.12, from the same quarter-wave table as sin_1. About 190
 * functions use it. */
s32 rcos(s32 angle) {
    s32 result;
    if (angle < 0)
        angle = -angle;
    angle &= (ONE - 1);
    if (angle <= (ONE / 2)) {
        if (angle <= (ONE / 4))
            result = g_psyq_gte_sine_quarter_table[(ONE / 4) - angle];
        else
            result = -g_psyq_gte_sine_quarter_table[angle - (ONE / 4)];
    } else {
        if (angle > (3 * ONE / 4))
            result = g_psyq_gte_sine_quarter_table[angle - (3 * ONE / 4)];
        else
            result = -g_psyq_gte_sine_quarter_table[(3 * ONE / 4) - angle];
    }
    return result;
}
