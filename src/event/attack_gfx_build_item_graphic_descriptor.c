#include "fft/event_attack.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Fill a 16x16 item icon descriptor: VRAM u/v, size, CLUT and texture page. */
void attack_gfx_build_item_graphic_descriptor(menu_icon_source_t* output, u32 item_id) {
    SPRT graphic;

    battle_gfx_set_item_icon_sprite(&graphic, item_id);
    output->u = graphic.u0;
    output->v = graphic.v0;
    output->w = 16;
    output->h = 16;
    output->clut = graphic.clut;
    output->tpage = GetTPage(0, 0, 0x380, 0x120);
}
