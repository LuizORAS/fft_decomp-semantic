#include "fft/event_equip.h"
#include "psx/types.h"

/* Clear the 6-byte selection record at `index`. */
void equip_menu_clear_selection_record(s32 index) {
    memset(&g_equip_menu_selection_entries[index], 0, sizeof(g_equip_menu_selection_entries[index]));
}
