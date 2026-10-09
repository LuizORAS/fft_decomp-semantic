#include "fft/event_equip.h"
#include "psx/types.h"

void equip_gfx_enqueue_draw_mode(s32 dfe, s32 dtd, s32 tpage, RECT* tw, s32 idx) {
    DR_MODE* primitive;

    primitive = &g_equip_gfx_context->draw_modes[g_equip_gfx_draw_mode_count++];
    SetDrawMode(primitive, dfe, dtd, tpage, tw);
    addPrim(&g_equip_gfx_context->otag[idx], primitive);
}
