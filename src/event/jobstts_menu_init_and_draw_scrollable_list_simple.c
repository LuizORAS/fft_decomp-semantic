#include "fft/event_jobstts.h"

/* Set up a scrollable list on `entries` with its cursor only (jobstts_menu_init_scrollable_list_core),
 * then draw it with display script `script`. */
void jobstts_menu_init_and_draw_scrollable_list_simple(
    s16* entries, s32 selected_index, u8* text_table, const menu_instruction_t* commands) {
    jobstts_menu_init_scrollable_list_core(entries, selected_index, text_table);
    jobstts_menu_draw_scrollable_list(commands);
}
