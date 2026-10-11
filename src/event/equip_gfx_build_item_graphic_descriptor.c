#include "fft/event_equip.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Fill a 16x16 item icon descriptor: VRAM u/v, size, CLUT and texture page. */
void equip_gfx_build_item_graphic_descriptor(menu_icon_source_t* descriptor, s32 item_id) {
    SPRT graphic;

    battle_gfx_set_item_icon_sprite(&graphic, item_id);
    descriptor->u = graphic.u0;
    descriptor->v = graphic.v0;
    descriptor->w = 0x10;
    descriptor->h = 0x10;
    descriptor->clut = graphic.clut;
    descriptor->tpage = GetTPage(0, 0, 0x380, 0x120);
}
