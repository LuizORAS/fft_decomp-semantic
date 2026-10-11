#include "fft/battle.h"
#include "psx/types.h"

/* Counts the decimal digits of the magnitude (at least one), not a minus sign. */
s32 battle_text_count_decimal_digits(s32 value) {
    s32 digits;

    value /= 10;
    digits = 1;
    if (value != 0) {
        do {
            value /= 10;
            digits++;
        } while (value != 0);
    }
    return digits;
}
