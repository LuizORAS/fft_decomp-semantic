#include "fft/script.h"

/* UnlockDate (EVENT_OPCODE_UNLOCK_DATE): store a date, day (5 bits) then month (4 bits), in the
 * 9-bit slot record of bitset. The interpreter records today's date for a treasure in
 * g_main_treasure_acquisition_date_bits. */
void script_unlock_date_event_instruction(u32* bits, s32 record, s32 month, s32 day) {
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
