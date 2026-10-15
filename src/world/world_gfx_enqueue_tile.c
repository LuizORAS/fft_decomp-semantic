#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Take the next TILE from the pool, colour it, set its semi-transparency and link it into
 * ordering-table entry ot_index (libgpu addPrim). */
void world_gfx_enqueue_tile(RECT* box, u8* rgb, u8 semi_trans, s32 ot_index) {
    TILE* tile = &g_world_gfx_active_packet_buffer->tiles[g_world_gfx_tile_count++];

    tile->r0 = rgb[0];
    tile->g0 = rgb[1];
    tile->b0 = rgb[2];
    SetSemiTrans(tile, semi_trans);
    tile->x0 = box->x + 0x80;
    tile->y0 = box->y;
    tile->w = box->w;
    tile->h = box->h;
    addPrim(&g_world_gfx_active_packet_buffer->otag[ot_index], tile);
}
