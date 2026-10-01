#include "fft/event_bunit.h"

void bunit_menu_init_and_draw_scrollable_list(s32 a0, s32 a1, s32 a2, s32 repeat_period, u8* script) {
    /* This caller forwards its own s32 parameter untouched; the definition's
     * narrow s16 scroll_base_index would truncate it at the call. */
    ((void (*)(s32, s32, s32, s32))bunit_menu_init_scrollable_list)(a0, a1, a2, repeat_period);
    bunit_menu_draw_scrollable_list(script);
}
