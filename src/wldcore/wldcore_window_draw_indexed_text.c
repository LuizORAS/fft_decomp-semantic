#include "fft/wldcore.h"

/* Draw the text that word `index` of a sound novel history level names (counted from the table's
 * word offset) centred in the level's window, as text 0x8800 + id; -1 draws nothing.
 *
 * The word is found through a variable record-relative offset, not a fixed struct field. */
void wldcore_window_draw_indexed_text(wldcore_window_text_table_t* table, s32 index) {
    s32 text_id = ((s32*)table)[index + table->table_word_offset];

    if (text_id != -1) {
        wldcore_window_draw_centered_text(table->window_index, text_id + TEXT_ID_WORLD_MESSAGE_BASE);
    }
}
