#include "fft/event_equip.h"

/* EQUIP.OUT 001c41dc - Build the icon rect for the item in menu slot `index`. */
world_item_icon_source_t* equip_item_build_type_icon_source(s32 index) {
    world_item_icon_source_t* out = &g_equip_item_row_icon_rect;
    equip_item_get_type_icon_rect(equip_item_get_type(g_equip_item_list_entries[index]), out);
    out->clut = g_equip_text_metric_2;
    out->tpage = g_equip_menu_cursor_texture_page;
    return out;
}
