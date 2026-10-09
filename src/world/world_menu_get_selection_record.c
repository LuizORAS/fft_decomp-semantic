#include "fft/world.h"
#include "psx/types.h"

void world_menu_get_selection_record(s32 index, s16* selected_index, s16* scroll_base_index, s16* entries) {
    s32 entry_id;
    s32 found_end;
    s32 i;

    *selected_index = g_world_menu_selection_records[index].selected_index;
    *scroll_base_index = g_world_menu_selection_records[index].scroll_base_index;
    entry_id = g_world_menu_selection_records[index].entry_id;
    found_end = 0;
    for (i = 0; i < *selected_index; i++) {
        if (entries[i] == -1) {
            found_end = 1;
        }
    }
    if (entry_id == entries[*selected_index] && found_end == 0) {
        return;
    }
    for (i = 0; entries[i] != -1; i++) {
        if ((entries[i] & 0x3ff) == entry_id) {
            *selected_index = i;
            return;
        }
    }
    *selected_index = 0;
    *scroll_base_index = 0;
}
