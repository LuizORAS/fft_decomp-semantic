#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Takes the next DR_MOVE packet from the pool, sets it up to copy `src`
 * to (x, y) and links it into ordering-table entry ot_index (libgpu addPrim). */
void world_gfx_enqueue_draw_move(const RECT* src, s32 x, s32 y, s32 ot_index) {
    DR_MOVE* move = &g_world_gfx_active_packet_buffer->draw_moves[g_world_gfx_draw_move_count++];

    SetDrawMove(move);
    move->sx = src->x;
    move->sy = src->y;
    move->x0 = x;
    move->y0 = y;
    move->w = src->w;
    move->h = src->h;
    addPrim(&g_world_gfx_active_packet_buffer->otag[ot_index], move);
}
