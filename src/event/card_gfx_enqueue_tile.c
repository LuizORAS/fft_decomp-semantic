#include "fft/event_card.h"
#include "psx/types.h"

/* Takes the next TILE packet from the pool, fills it from the rectangle
 * (x offset by 0x80) and the RGB triple and links it into ordering-table
 * entry otag_index. */
void card_gfx_enqueue_tile(const RECT* rect, const u8* color, s32 semitrans, s32 otag_index) {
    TILE* tile;

    tile = &g_card_gfx_context->tiles[g_card_gfx_tile_count++];
    tile->r0 = color[0];
    tile->g0 = color[1];
    tile->b0 = color[2];
    SetSemiTrans(tile, semitrans & 0xff);
    tile->x0 = rect->x + 0x80;
    tile->y0 = rect->y;
    tile->w = rect->w;
    tile->h = rect->h;
    addPrim(&g_card_gfx_context->otag[otag_index], tile);
}
