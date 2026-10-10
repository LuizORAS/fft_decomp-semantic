#include "fft/wldcore.h"

/* Skip -1, otherwise draw the text id in text section 17 (0x8800).
 * This is a variable record-relative word offset, not a fixed struct field. */
void wldcore_window_draw_indexed_text(wldcore_window_text_table_t* table, s32 index) {
    s32 text_id = ((s32*)table)[index + table->table_word_offset];

    if (text_id != -1) {
        wldcore_window_draw_centered_text(table->window_index, text_id + TEXT_ID_WORLD_MESSAGE_BASE);
    }
}
