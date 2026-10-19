#include "fft/main.h"

/* Roll 0..maximum-1 and compare it with threshold: returns 0 when the roll is
 * below threshold and 1 otherwise. An event tested with `== 0` therefore has
 * threshold-in-maximum odds. */
s32 main_util_roll_pass_fail(s32 maximum, s32 threshold) {
    s32 roll = rand() * maximum / 0x8000;

    return roll >= threshold;
}
