#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Port debt (QUIRKS.md): data is a pointer, but the core takes it as s32. */
void jobstts_menu_init_scrollable_list(const s16* entries, s32 selected_index, s32 value, const void* data) {
    jobstts_menu_init_scrollable_list_core((s16*)entries, selected_index, (s32)data);
    g_jobstts_menu_scroll_base_index = value;
}
