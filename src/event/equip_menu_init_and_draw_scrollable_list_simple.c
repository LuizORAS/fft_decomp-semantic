#include "fft/event_equip.h"
#include "psx/types.h"

void equip_menu_init_and_draw_scrollable_list_simple(s32 entries, s32 selected_index, s32 text_table, s32 script) {
    equip_menu_init_scrollable_list_core((s16*)entries, selected_index, (const void*)text_table);
    equip_menu_draw_scrollable_list((u8*)script);
}
