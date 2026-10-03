#include "fft/event_equip.h"
#include "psx/types.h"

void equip_menu_init_and_draw_scrollable_list_simple(s16* entries, s32 selected_index, void* text_table, u8* script) {
    equip_menu_init_scrollable_list_core(entries, selected_index, text_table);
    equip_menu_draw_scrollable_list(script);
}
