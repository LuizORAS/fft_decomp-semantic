#include "fft/battle.h"
#include "psx/types.h"

/* Point all 32 text sections at a text file in memory, whose table of 32 section offsets is `offsets`,
 * counted from the end of the table. BUNIT, EQUIP and JOBSTTS bind their help text this way, between
 * battle_text_save_section_pointers and battle_text_restore_section_pointers. */
void battle_text_relocate_pointer_table(const u32* offsets) {
    s32 index;
    const u8* text_data;

    index = 0;
    text_data = (const u8*)offsets + sizeof(u32) * 32;
    do {
        g_battle_text_section_pointers[index] = (u8*)(text_data + offsets[index]);
        index++;
    } while (index < 32);
}
