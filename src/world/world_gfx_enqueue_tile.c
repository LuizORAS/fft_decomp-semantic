#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Takes the next TILE from the pool, colours it, sets semi-transparency and
 * links it into ordering-table entry ot_index (libgpu addPrim). */
/* Target 0x8012bdbc. */
void world_gfx_enqueue_tile(RECT* box, rgb24_t* rgb, u8 semi_trans, s32 ot_index) {
    TILE* tile = &g_world_gfx_active_packet_buffer->tiles[g_world_gfx_tile_count++];

    tile->r0 = rgb->r;
    tile->g0 = rgb->g;
    tile->b0 = rgb->b;
    SetSemiTrans(tile, semi_trans);
    tile->x0 = box->x + 0x80;
    tile->y0 = box->y;
    tile->w = box->w;
    tile->h = box->h;
    addPrim(&g_world_gfx_active_packet_buffer->otag[ot_index], tile);
}
