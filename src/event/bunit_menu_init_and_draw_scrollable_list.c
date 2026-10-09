#include "fft/event_bunit.h"

void bunit_menu_init_and_draw_scrollable_list(s32 a0, s32 a1, s32 a2, s32 repeat_period, u8* script) {
    bunit_menu_init_scrollable_list(a0, a1, a2, repeat_period);
    bunit_menu_draw_scrollable_list(script);
}
