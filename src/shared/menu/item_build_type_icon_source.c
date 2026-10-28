#include "fft/event_equip.h"
#include "fft/menu.h"

/* Build the 12x12 type icon of the item in list row `index` of a shop or inventory list
 * (g_menu_item_type_icon_source, with the menu's CLUT and texture page) and return it: a row callback
 * of the item list. */
battle_menu_icon_source_t* item_build_type_icon_source(s32 index) {
    battle_menu_icon_source_t* out = &g_menu_item_type_icon_source;
    equip_item_get_type_icon_rect(equip_item_get_type(g_equip_item_list_entries[index]), out);
    out->clut = g_menu_palette_clut;
    out->tpage = g_menu_cursor_texture_page;
    return out;
}
