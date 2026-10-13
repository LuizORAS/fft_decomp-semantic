#include "fft/event_equip.h"
#include "psx/types.h"

/* Forget every saved list position (the five 6-byte records of g_equip_menu_selection_entries); twin of
 * world_menu_clear_selection_records. */
void equip_menu_clear_selection_records(void) {
    memset(g_equip_menu_selection_entries, 0, 0x1E);
}
