#include "fft/event_equip.h"
#include "psx/types.h"

/* Takes the next DR_MOVE packet from the pool, sets it up to copy `rect`
 * to (u, v) and links it into ordering-table entry otag_index. */
void equip_gfx_enqueue_draw_move(const RECT* rect, s32 u, s32 v, s32 otag_index) {
    DR_MOVE* prim;

    prim = &g_equip_gfx_context->moves[g_equip_gfx_draw_move_count++];
    SetDrawMove(prim);
    prim->x0 = rect->x;
    prim->y0 = rect->y;
    prim->x1 = u;
    prim->y1 = v;
    prim->w = rect->w;
    prim->h = rect->h;
    addPrim(&g_equip_gfx_context->otag[otag_index], prim);
}
