#include "fft/event_jobstts.h"

void jobstts_menu_clear_selection_record(s32 index) {
    memset(&g_jobstts_menu_selection_records[index], 0, sizeof(bunit_menu_selection_record_t));
}
