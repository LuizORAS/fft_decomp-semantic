#include "fft/world.h"
#include "psx/types.h"

/* Store an unlock date into a bitmap of 9-bit records: five bits of day
 * followed by four bits of month. */
void world_script_unlockdate_event_instruction(u32* bits, s32 record, s32 month, s32 day) {
    s32 base;
    s32 i;
    u8 mask;

    base = record * 9;
    mask = 1;
    for (i = 0; i < 5; i++) {
        world_script_assign_bitset_flag(bits, base + i, day & mask);
        mask <<= 1;
    }
    mask = 1;
    for (i = 0; i < 4; i++) {
        s32 bit = i + 5;
        world_script_assign_bitset_flag(bits, base + bit, month & mask);
        mask <<= 1;
    }
}
