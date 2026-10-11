#include "fft/event_equip.h"

/* Texture cell of an item type's 12x12 icon. */
void equip_item_get_type_icon_rect(s32 item_type, menu_icon_source_t* output) {
    output->u = g_equip_item_type_icon_coords[item_type][0];
    output->v = g_equip_item_type_icon_coords[item_type][1];
    output->w = 12;
    output->h = 12;
}
