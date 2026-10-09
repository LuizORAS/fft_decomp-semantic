#include "fft/world.h"

void world_menu_set_selection_record(s32 index, s32 selected_index, s32 scroll_base_index, s16* entries) {
    g_world_menu_selection_records[index].selected_index = selected_index;
    g_world_menu_selection_records[index].scroll_base_index = scroll_base_index;
    g_world_menu_selection_records[index].entry_id = entries[selected_index] & 0x3FF;
}
