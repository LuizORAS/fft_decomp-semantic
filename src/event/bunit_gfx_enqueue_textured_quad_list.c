#include "fft/event_bunit.h"
#include "psx/types.h"

void bunit_gfx_enqueue_textured_quad_list(const battle_menu_sprite_cell_t* quads, const u8* color, s32 semitrans,
    u16 texture_page, u16 clut, s32 otag_index, s32 count) {
    POLY_FT4* poly;

    for (count -= 1; count >= 0; count--) {
        poly = &g_bunit_gfx_context->textured_quads[g_bunit_gfx_poly_ft4_count++];

        if (color != 0) {
            SetShadeTex(poly, 0);
            poly->r0 = color[0];
            poly->g0 = color[1];
            poly->b0 = color[2];
        } else {
            SetShadeTex(poly, 1);
        }
        SetSemiTrans(poly, semitrans);

        poly->x0 = quads->x + 0x80;
        poly->y0 = quads->y;
        poly->x1 = quads->w + (quads->x + 0x80);
        poly->y1 = quads->y;
        poly->x2 = quads->x + 0x80;
        poly->y2 = quads->y + quads->h;
        poly->x3 = quads->w + (quads->x + 0x80);
        poly->y3 = quads->y + quads->h;
        poly->u0 = quads->u;
        poly->v0 = quads->v;
        poly->u1 = quads->u + quads->w;
        poly->v1 = quads->v;
        poly->u2 = quads->u;
        poly->v2 = quads->v + quads->h;
        poly->u3 = quads->u + quads->w;
        poly->v3 = quads->v + quads->h;
        poly->clut = clut;
        poly->tpage = texture_page;

        setaddr(poly, g_bunit_gfx_context->otag[otag_index]);
        catPrim(&g_bunit_gfx_context->otag[otag_index], poly);
        quads++;
    }
}
