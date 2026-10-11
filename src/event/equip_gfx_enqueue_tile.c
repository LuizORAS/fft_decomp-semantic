#include "fft/event_equip.h"
#include "psx/types.h"

/* Takes the next TILE packet from the pool, fills it from the rectangle
 * (x offset by 0x80) and the RGB triple and links it into ordering-table
 * entry otag_index. */
void equip_gfx_enqueue_tile(const RECT* rect, const rgb24_t* color, u8 semitrans, s32 otag_index) {
    TILE* tile;

    tile = &g_equip_gfx_context->tiles[g_equip_gfx_tile_count++];
    tile->r0 = color->r;
    tile->g0 = color->g;
    tile->b0 = color->b;
    SetSemiTrans(tile, semitrans & 0xff);
    tile->x0 = rect->x + 0x80;
    tile->y0 = rect->y;
    tile->w = rect->w;
    tile->h = rect->h;
    addPrim(&g_equip_gfx_context->otag[otag_index], tile);
}
