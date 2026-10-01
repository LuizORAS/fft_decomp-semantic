#include "fft/event_bunit.h"

void bunit_menu_init_and_draw_scrollable_list_simple(s32 entries, s32 selected_index, s32 text_table, u8* script) {
    bunit_menu_init_scrollable_list_core((s16*)entries, selected_index, text_table);
    bunit_menu_draw_scrollable_list(script);
}
