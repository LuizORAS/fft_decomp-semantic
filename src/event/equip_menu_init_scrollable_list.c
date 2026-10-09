#include "fft/event_equip.h"
#include "psx/types.h"

void equip_menu_init_scrollable_list(s16* entries, s32 selected_index, s32 scroll_base_index, u8* text_table) {
    equip_menu_init_scrollable_list_core(entries, selected_index, text_table);
    g_equip_menu_scroll_base_index = scroll_base_index;
}
