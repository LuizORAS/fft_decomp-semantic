#include "fft/event_jobstts.h"
#include "psx/types.h"

void jobstts_menu_init_scrollable_list(s16* entries, s32 selected_index, s32 scroll_base_index, u8* text_table) {
    jobstts_menu_init_scrollable_list_core(entries, selected_index, text_table);
    g_jobstts_menu_scroll_base_index = scroll_base_index;
}
