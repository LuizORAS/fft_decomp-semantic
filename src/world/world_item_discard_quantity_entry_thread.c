#include "fft/world.h"
#include "psx/types.h"

/* Thread entry for the item menu's quantity entry: runs world_menu_number_entry_thread. No C code
 * calls it; the WORLD menu entry record just before g_world_item_quantity_change_menu names it (the
 * word at 0x8018cb88). */
void world_item_discard_quantity_entry_thread(void) {
    world_menu_number_entry_thread();
}
