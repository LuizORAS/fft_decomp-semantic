#include "fft/main.h"

/* Same as main_util_copy_byte_data: a byte-identical copy at 0x8005cc64, which the unit generation and
 * update routines (main_unit_*) call. */
void main_util_copy_bytes(const void* source, void* destination, int count) {
    const u8* src = source;
    u8* dst = destination;
    int i;

    for (i = 0; i < count; i++) {
        *dst++ = *src++;
    }
}
