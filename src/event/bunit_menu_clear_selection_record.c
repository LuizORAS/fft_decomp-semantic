#include "fft/event_bunit.h"
#include "psx/types.h"

/* Clear the 6-byte selection record at `index`. */
void bunit_menu_clear_selection_record(s32 index) {
    memset(&g_bunit_menu_selection_records[index], 0, sizeof(bunit_menu_selection_record_t));
}
