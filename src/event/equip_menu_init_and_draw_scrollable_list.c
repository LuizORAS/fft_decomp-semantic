#include "fft/event_equip.h"
#include "psx/types.h"

void equip_menu_init_and_draw_scrollable_list(
    s16* entries, s32 selected_index, s32 scroll_base_index, u8* text_table, const u8* commands) {
    equip_menu_init_scrollable_list(entries, selected_index, scroll_base_index, text_table);
    equip_menu_draw_scrollable_list(commands);
}
