#include "fft/event_bunit.h"

/* Set up a scrollable list on `entries` with its cursor only (bunit_menu_init_scrollable_list_core),
 * then draw it with display script `script`. */
void bunit_menu_init_and_draw_scrollable_list_simple(
    s16* entries, s32 selected_index, u8* text_table, const menu_instruction_t* script) {
    bunit_menu_init_scrollable_list_core(entries, selected_index, text_table);
    bunit_menu_draw_scrollable_list(script);
}
