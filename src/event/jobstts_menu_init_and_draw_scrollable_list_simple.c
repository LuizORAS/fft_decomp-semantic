#include "fft/event_jobstts.h"

void jobstts_menu_init_and_draw_scrollable_list_simple(
    s16* entries, s32 selected_index, u8* text_table, const u8* commands) {
    jobstts_menu_init_scrollable_list_core(entries, selected_index, text_table);
    jobstts_menu_draw_scrollable_list(commands);
}
