#include "fft/event_bunit.h"
#include "psx/types.h"

void bunit_gfx_enqueue_draw_move(const RECT* rect, s32 u, s32 v, s32 otag_index) {
    DR_MOVE* prim;

    prim = &g_bunit_gfx_context->draw_moves[g_bunit_gfx_draw_move_count++];
    SetDrawMove(prim);
    prim->x0 = rect->x;
    prim->y0 = rect->y;
    prim->x1 = u;
    prim->y1 = v;
    prim->w = rect->w;
    prim->h = rect->h;
    addPrim(&g_bunit_gfx_context->otag[otag_index], prim);
}
