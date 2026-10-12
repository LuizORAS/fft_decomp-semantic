#include "fft/menu.h"

/* Widen count bytes into 16-bit menu values; a nonpositive count writes nothing. */
void menu_copy_u8_to_u16(u16* destination, const u8* source, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        *destination = *source;
        source++;
        destination++;
    }
}
