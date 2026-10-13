#include "fft/event_jobstts.h"

/* Set up a scrollable list on `entries` with its cursor and scroll base (jobstts_menu_init_scrollable_list),
 * then draw it with display script `script`. */
void jobstts_menu_init_and_draw_scrollable_list(
    s16* entries, s32 selected_index, s32 scroll_base_index, u8* text_table, const u8* commands) {
    jobstts_menu_init_scrollable_list(entries, selected_index, scroll_base_index, text_table);
    jobstts_menu_draw_scrollable_list(commands);
}
