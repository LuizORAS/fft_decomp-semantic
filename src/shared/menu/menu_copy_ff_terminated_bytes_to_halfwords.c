#include "fft/menu.h"

/* Widen a 0xff-terminated byte list into 16-bit values, without the terminator, and
 * return the number of entries copied. WORLD compiles this text too. */
s32 menu_copy_ff_terminated_bytes_to_halfwords(u16* destination, const u8* source) {
    s32 count = 0;

    for (;;) {
        if (*source == 0xFF) {
            return count;
        }
        *destination = *source;
        source++;
        count++;
        destination++;
    }
}
