#include "fft/wldcore.h"

/* Draw the text that 32-bit value `index` of a sound novel history level names (counted from the table's
 * offset) centred in the level's window, as text 0x8800 + id; -1 draws nothing.
 *
 * The value is found through a variable record-relative offset, not a fixed struct field. */
void wldcore_window_draw_indexed_text(wldcore_menu_sound_novel_history_level_t* level, s32 index) {
    /* Named text-array access reverses the final address-add operands. */
    s32 text_id = ((s32*)level)[index + level->scroll];

    if (text_id != -1) {
        wldcore_window_draw_centered_text(level->render_index, text_id + TEXT_ID_WORLD_MESSAGE_BASE);
    }
}
