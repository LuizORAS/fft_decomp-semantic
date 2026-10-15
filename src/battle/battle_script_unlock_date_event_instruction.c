#include "fft/battle.h"

/* UnlockDate (EVENT_OPCODE_UNLOCK_DATE): store a date, day (5 bits) then month (4 bits), in the
 * 9-bit slot date_index of bitset. The interpreter records today's date for a treasure in
 * g_main_treasure_acquisition_date_bits. */
void battle_script_unlock_date_event_instruction(u32* bitset, s32 date_index, s32 month, s32 day) {
    s32 first_bit;
    s32 i;
    s32 mask;

    first_bit = date_index * 9;
    mask = 1;
    i = 0;
    do {
        battle_script_assign_bitset_flag(bitset, first_bit + i, day & (u8)mask);
        i++;
        mask *= 2;
    } while (i < 5);
    mask = 1;
    i = 0;
    do {
        s32 slot = i + 5;
        battle_script_assign_bitset_flag(bitset, first_bit + slot, month & (u8)mask);
        i++;
        mask *= 2;
    } while (i < 4);
}
