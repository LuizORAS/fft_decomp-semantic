#include "fft/event_bunit.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Fill a 16x16 item icon descriptor: VRAM u/v, size, CLUT and texture page. */
void bunit_gfx_build_item_graphic_descriptor(battle_menu_status_panel_graphic_descriptor_t* descriptor, s32 item_id) {
    SPRT graphic;

    battle_gfx_set_item_icon_sprite(&graphic, item_id);
    descriptor->x_load_location = graphic.u0;
    descriptor->y_load_location = graphic.v0;
    descriptor->width = 0x10;
    descriptor->height = 0x10;
    descriptor->clut = graphic.clut;
    descriptor->tpage = GetTPage(0, 0, 0x380, 0x120);
}
