#include "fft/event_jobstts.h"

/* Forget every saved list position (g_jobstts_menu_selection_records); twin of
 * world_menu_clear_selection_records. */
void jobstts_menu_clear_selection_records(void) {
    memset(g_jobstts_menu_selection_records, 0, sizeof(g_jobstts_menu_selection_records));
}
