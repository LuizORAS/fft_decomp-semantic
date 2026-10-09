#include "fft/world.h"

void world_menu_init_scrollable_list(const s16* values, s32 value, s32 selected, u8* text_table) {
    world_menu_init_scrollable_list_core(values, value, text_table);
    g_world_menu_scroll_offset = selected;
}
