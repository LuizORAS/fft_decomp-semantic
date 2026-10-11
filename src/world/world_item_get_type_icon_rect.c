#include "fft/world.h"
#include "psx/types.h"
/* Texture cell of an item type's 12x12 icon. */
void world_item_get_type_icon_rect(s32 index, menu_icon_source_t* out) {
    out->u = g_world_item_type_icon_origins[index].u;
    out->v = g_world_item_type_icon_origins[index].v;
    out->w = 0xC;
    out->h = 0xC;
}
