#include "fft/battle.h"
#include "psx/types.h"

/* Returns the start of a text entry. The text id holds the section in its high
 * part and the entry number in its low part (TEXT_ID_SECTION_MASK and
 * TEXT_ID_ENTRY_MASK). */
u8* battle_text_find_entry(u32 text_id) {
    u32 entry_id;

    entry_id = text_id;
    return (u8*)battle_text_skip_to_entry(
        g_battle_text_section_pointers[(entry_id & TEXT_ID_SECTION_MASK) >> TEXT_ID_SECTION_SHIFT],
        entry_id & TEXT_ID_ENTRY_MASK);
}
