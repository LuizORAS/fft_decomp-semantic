#include "fft/event_bunit.h"

void bunit_menu_init_and_draw_scrollable_list(
    s32 entries, s32 selected_index, s32 scroll_base_index, s32 text_table, u8* script) {
    /* This caller forwards its own s32 parameter untouched; the definition's
     * narrow s16 scroll_base_index would truncate it at the call. */
    ((void (*)(s32, s32, s32, s32))bunit_menu_init_scrollable_list)(
        entries, selected_index, scroll_base_index, text_table);
    bunit_menu_draw_scrollable_list(script);
}
