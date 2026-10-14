#include "fft/event_equip.h"
#include "fft/menu.h"

/* EQUIP.OUT 001c41dc - Build the icon rect for the item in menu slot `index`. */
menu_icon_source_t* equip_item_build_type_icon_source(s32 index) {
    menu_icon_source_t* out = &g_equip_item_row_icon_rect;
    equip_item_get_type_icon_rect(equip_item_get_type(g_equip_item_list_entries[index]), out);
    out->clut = g_menu_palette_clut;
    out->tpage = g_equip_menu_cursor_texture_page;
    return out;
}
