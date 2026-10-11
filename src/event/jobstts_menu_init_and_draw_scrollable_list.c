#include "fft/event_jobstts.h"

void jobstts_menu_init_and_draw_scrollable_list(
    s16* entries, s32 selected_index, s32 scroll_base_index, u8* text_table, const menu_instruction_t* commands) {
    jobstts_menu_init_scrollable_list(entries, selected_index, scroll_base_index, text_table);
    jobstts_menu_draw_scrollable_list(commands);
}
