#include "fft/wldcore.h"

/* Assign one bit without changing the rest of its storage word. */
void wldcore_set_bit_value(u32* bits, s32 index, s32 set) {
    u32* base;
    u32 mask;
    s32 word_index;

    base = bits;
    word_index = index / 32;
    mask = 1U << (index & 31);
    if (set != 0) {
        base[word_index] |= mask;
    } else {
        base[word_index] &= ~mask;
    }
}
