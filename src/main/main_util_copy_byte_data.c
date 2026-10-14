#include "fft/main.h"

/* Copy `count` bytes from `source` to `destination`, one at a time (the source comes first, unlike
 * memcpy). BATTLE's action, AI, formula, movement and status code copies its records with it. */
void main_util_copy_byte_data(const void* source, void* destination, s32 count) {
    const u8* src = source;
    u8* dst = destination;
    s32 index = 0;

    while (index < count) {
        *dst++ = *src++;
        index++;
    }
}
