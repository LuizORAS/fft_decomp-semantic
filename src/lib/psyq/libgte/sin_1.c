#include "psx/libgte.h"

/* The sine of `angle` (0-4095) from LIBGTE's quarter-wave table, read by quadrant. rsin calls it. */
s32 sin_1(s32 angle) {
    s32 result;
    if (angle <= (ONE / 2)) {
        if (angle <= (ONE / 4))
            result = g_psyq_gte_sine_quarter_table[angle];
        else
            result = g_psyq_gte_sine_quarter_table[(ONE / 2) - angle];
    } else {
        if (angle > (3 * ONE / 4))
            result = -g_psyq_gte_sine_quarter_table[ONE - angle];
        else
            result = -g_psyq_gte_sine_quarter_table[angle - (ONE / 2)];
    }
    return result;
}
