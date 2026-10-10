#include "fft/event_bunit.h"
#include "psx/types.h"

/* Queue a drawing-area primitive, shifting it to the back buffer when needed. */
void bunit_gfx_enqueue_draw_area(RECT* rect, s32 idx) {
    DR_AREA* p;

    if (g_bunit_gfx_drawenv_y < 0x64) {
        rect->y += 0xF0;
    }

    p = &g_bunit_gfx_context->draw_areas[g_bunit_gfx_draw_area_count++];
    SetDrawArea(p, rect);
    addPrim(&g_bunit_gfx_context->otag[idx], p);
}
