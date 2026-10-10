#include "fft/event_equip.h"
#include "psx/types.h"

/*
 * Read back the cursor (*p1) and scroll base (*p2) saved for list `idx` and
 * check them against the rebuilt -1-terminated `list`.
 *
 * They are kept when the saved entry id is still at the cursor row. Otherwise
 * the cursor moves to the row that holds that id (low ten bits), keeping the
 * scroll base, or both reset to 0 when the id is gone.
 */
void equip_menu_restore_list_selection(s32 idx, s16* p1, s16* p2, s16* list) {
    s32 i;
    s32 found;
    s32 key;

    found = 0;
    *p1 = g_equip_menu_selection_entries[idx].selected_index;
    *p2 = g_equip_menu_selection_entries[idx].scroll_base_index;
    key = g_equip_menu_selection_entries[idx].entry_id;

    for (i = 0; i < *p1; i++) {
        if (list[i] == -1) {
            found = 1;
        }
    }

    if (key == list[*p1] && !found) {
        return;
    }

    for (i = 0; list[i] != -1; i++) {
        if ((list[i] & 0x3FF) == key) {
            *p1 = i;
            return;
        }
    }

    *p1 = 0;
    *p2 = 0;
}
