#include "fft/world.h"
#include "psx/types.h"

/* Set or clear one bit in a 32-bit-element bitmap. */
void world_script_assign_bitset_flag(u32* bits, s32 index, s32 set) {
    s32 word = index / 32;
    u32 mask = 1U << (index & 0x1F);

    if (set != 0) {
        bits[word] |= mask;
    } else {
        bits[word] &= ~mask;
    }
}
