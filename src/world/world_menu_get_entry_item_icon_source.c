#include "fft/world.h"
#include "psx/types.h"

/* Menu callback: the icon source of the item in menu row `index`, built in a
 * shared record. */
menu_icon_source_t* world_menu_get_entry_item_icon_source(s32 index) {
    world_gfx_build_item_graphic_descriptor(&g_world_menu_entry_item_icon_source, (u8)g_world_menu_entry_ids[index]);
    return &g_world_menu_entry_item_icon_source;
}
