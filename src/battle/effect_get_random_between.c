#include "fft/battle_effect.h"
#include "psx/libc.h"

/* Random integer between the two arguments, given in either order: at least the
 * smaller and less than the larger. Equal arguments return that value without
 * drawing a random number. */
s32 effect_get_random_between(s32 value_a, s32 value_b) {
    s32 remainder;
    s32 random_value;
    s32 result;

    if (value_a == value_b) {
        return value_a;
    }
    if (value_b < value_a) {
        random_value = rand();
        remainder = random_value % (value_a - value_b);
        result = remainder + value_b;
    } else {
        random_value = rand();
        remainder = random_value % (value_b - value_a);
        result = remainder + value_a;
    }
    return result;
}
