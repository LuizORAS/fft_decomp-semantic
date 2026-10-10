#include "fft/event_card.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Fill the context's single DR_MODE packet and link it into ordering-table
 * entry otag_index. */
void card_gfx_set_draw_mode_and_link_ot(s32 dfe, s32 dtd, s32 tpage, RECT* texture_window, s32 otag_index) {
    card_graphics_context_t* context = g_card_gfx_context;
    DR_MODE* mode = &context->draw_mode;

    SetDrawMode(mode, dfe, dtd, tpage, texture_window);
    addPrim(&g_card_gfx_context->otag[otag_index], mode);
}
