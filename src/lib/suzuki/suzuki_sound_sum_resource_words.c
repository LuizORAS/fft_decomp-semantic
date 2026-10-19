#include "psx/suzuki.h"

/* Sums every 32-bit value from the size field to the end of the resource. */
s32 suzuki_sound_sum_resource_words(suzuki_resource_prefix_t* resource) {
    u32* word;
    s32 count;
    s32 sum;

    word = &resource->size_bytes;
    count = (resource->size_bytes - 8) >> 2;
    sum = 0;
    do {
        sum += *word++;
    } while (--count != 0);
    return sum;
}
