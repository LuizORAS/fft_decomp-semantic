#include "fft/event_bunit.h"

void bunit_menu_init_and_draw_scrollable_list(
    s16* entries, s32 selected_index, s32 scroll_base_index, u8* text_table, u8* script) {
    bunit_menu_init_scrollable_list(entries, selected_index, scroll_base_index, text_table);
    bunit_menu_draw_scrollable_list(script);
}
