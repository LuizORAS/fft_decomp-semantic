#include "fft/world.h"
#include "psx/types.h"

enum {
    WORLD_TEXT_ENTRY_END = 0xFE,
    WORLD_TEXT_ENTRY_SEPARATOR = 0xF8,
};

/* Concatenate selected entries into one terminated WORLD text stream.
 *
 * Separators are optional; when requested, the final separator is replaced by
 * the stream terminator instead of being emitted after the last entry. */
void world_text_concatenate_entries(u8* text_table, u8* destination, const s16* text_ids, s32 separate_entries) {
    u8* source;
    s32 i;

    for (i = 0; text_ids[i] != -1; i++) {
        source = world_text_find_entry_by_index(text_table, text_ids[i] & TEXT_ID_ENTRY_MASK, 1);
        while (*source != WORLD_TEXT_ENTRY_END) {
            *destination++ = *source++;
        }
        if (separate_entries != 0) {
            *destination++ = WORLD_TEXT_ENTRY_SEPARATOR;
        }
    }
    if (separate_entries != 0) {
        destination--;
    }
    *destination = WORLD_TEXT_ENTRY_END;
}
