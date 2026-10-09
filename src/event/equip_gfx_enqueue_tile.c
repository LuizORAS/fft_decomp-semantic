#include "fft/event_equip.h"
#include "psx/types.h"

void equip_gfx_enqueue_tile(const RECT* rect, const u8* color, u8 semitrans, s32 otag_index) {
    TILE* tile;

    tile = &g_equip_gfx_context->tiles[g_equip_gfx_tile_count++];
    tile->r0 = color[0];
    tile->g0 = color[1];
    tile->b0 = color[2];
    SetSemiTrans(tile, semitrans & 0xff);
    tile->x0 = rect->x + 0x80;
    tile->y0 = rect->y;
    tile->w = rect->w;
    tile->h = rect->h;
    addPrim(&g_equip_gfx_context->otag[otag_index], tile);
}
