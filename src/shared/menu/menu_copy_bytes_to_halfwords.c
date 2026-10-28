#include "fft/menu.h"

/* Widen `count` bytes at `source` into 16-bit values at `destination` (a nonpositive count writes
 * nothing). WORLD compiles this text too. */
void menu_copy_bytes_to_halfwords(u16* destination, const u8* source, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        *destination = *source;
        source++;
        destination++;
    }
}
