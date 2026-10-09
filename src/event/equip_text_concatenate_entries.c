#include "fft/event_equip.h"
#include "psx/types.h"

/* Concatenate the text for each entry of a -1 terminated id list into dst,
   optionally separating entries with 0xF8, and terminate with 0xFE. */
void equip_text_concatenate_entries(const u8* text_table, u8* dst, s16* list, s32 separate) {
    const u8* src;
    s16* entry;

    if (*list != -1) {
        entry = list;
        do {
            src = equip_text_find_entry_by_index(text_table, *(u16*)entry & 0x7FF, 1);
            while (*src != 0xFE) {
                *dst = *src;
                src++;
                dst++;
            }
            entry++;
            if (separate != 0) {
                *dst = 0xF8;
                dst++;
            }
        } while (*entry != -1);
    }

    if (separate != 0) {
        dst--;
    }
    *dst = 0xFE;
}
