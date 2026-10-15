#include "fft/event_jobstts.h"

/* Forget the saved position of list `index` (one 6-byte record of g_jobstts_menu_selection_records). */
void jobstts_menu_clear_selection_record(s32 index) {
    memset(&g_jobstts_menu_selection_records[index], 0, sizeof(bunit_menu_selection_record_t));
}
