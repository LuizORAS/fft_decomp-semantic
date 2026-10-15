#include "fft/event_equip.h"

/* Build the 12x12 type icon of the item in list row index (g_equip_item_row_icon_rect, with the
 * menu's CLUT and texture page) and return it: a row callback of the item list. */
world_item_icon_source_t* equip_item_build_type_icon_source(s32 index) {
    world_item_icon_source_t* out = &g_equip_item_row_icon_rect;
    equip_item_get_type_icon_rect(equip_item_get_type(g_equip_item_list_entries[index]), out);
    out->clut = g_equip_menu_palette_clut;
    out->tpage = g_equip_menu_cursor_texture_page;
    return out;
}
