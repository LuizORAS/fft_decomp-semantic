#include "fft/event_equip.h"
#include "psx/types.h"

void equip_menu_clear_selection_records(void) {
    memset(g_equip_menu_selection_entries, 0, sizeof(g_equip_menu_selection_entries));
}
