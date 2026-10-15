#include "psx/libgte.h"

/* The sine of `angle` (0-4095) from LIBGTE's quarter-wave table, read by quadrant. rsin calls it. */
s32 sin_1(s32 angle) {
    s32 result;
    if (angle <= 2048) {
        if (angle <= 1024)
            result = g_psyq_gte_sine_quarter_table[angle];
        else
            result = g_psyq_gte_sine_quarter_table[2048 - angle];
    } else {
        if (angle > 3072)
            result = -g_psyq_gte_sine_quarter_table[4096 - angle];
        else
            result = -g_psyq_gte_sine_quarter_table[angle - 2048];
    }
    return result;
}
