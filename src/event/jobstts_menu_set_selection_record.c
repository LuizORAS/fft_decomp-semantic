#include "fft/event_jobstts.h"

void jobstts_menu_set_selection_record(s32 index, s32 selected_index, s32 scroll_index, s16* abilities) {
    g_jobstts_menu_selection_records[index].selected_index = selected_index;
    g_jobstts_menu_selection_records[index].scroll_base_index = scroll_index;
    g_jobstts_menu_selection_records[index].entry_id = abilities[selected_index] & 0x3ff;
}
