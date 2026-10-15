#include "fft/event_bunit.h"
#include "psx/types.h"

/* Save a list's cursor row and scroll base in selection record `index`,
 * together with the id (low ten bits) of the entry under the cursor. */
void bunit_menu_set_selection_record(s32 index, s32 field0, s32 field2, s16* lookup_table) {
    g_bunit_menu_selection_records[index].selected_index = field0;
    g_bunit_menu_selection_records[index].scroll_base_index = field2;
    g_bunit_menu_selection_records[index].entry_id = lookup_table[field0] & 0x3FF;
}
