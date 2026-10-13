#include "fft/script.h"

/* Store five day bits and four month bits in the indexed nine-bit date record. */
void script_unlockdate_event_instruction(u32* bits, s32 record, s32 month, s32 day) {
    s32 base;
    s32 i;
    u8 mask;

    base = record * 9;
    mask = 1;
    for (i = 0; i < 5; i++) {
        script_assign_bitset_flag(bits, base + i, day & mask);
        mask <<= 1;
    }
    mask = 1;
    for (i = 0; i < 4; i++) {
        s32 bit = i + 5;
        script_assign_bitset_flag(bits, base + bit, month & mask);
        mask <<= 1;
    }
}
