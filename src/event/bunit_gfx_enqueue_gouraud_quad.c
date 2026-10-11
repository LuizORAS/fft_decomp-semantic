#include "fft/event_bunit.h"
#include "psx/types.h"

/*
 * Append a four-color menu quad to the selected ordering-table entry.
 *
 * `pts` holds the four corners as x, y pairs and `color` four RGB triples.
 * Screen x coordinates use the menu's +0x80 origin.
 */
void bunit_gfx_enqueue_gouraud_quad(const upoint16_t* pts, rgb24_t* color, s32 semi, s32 idx) {
    POLY_G4* poly;

    poly = &g_bunit_gfx_context->gouraud_quads[g_bunit_gfx_poly_g4_count++];
    SetSemiTrans(poly, semi);
    poly->x0 = pts[0].x + 0x80;
    poly->y0 = pts[0].y;
    poly->x1 = pts[1].x + 0x80;
    poly->y1 = pts[1].y;
    poly->x2 = pts[2].x + 0x80;
    poly->y2 = pts[2].y;
    poly->x3 = pts[3].x + 0x80;
    poly->y3 = pts[3].y;
    poly->r0 = color[0].r;
    poly->g0 = color[0].g;
    poly->b0 = color[0].b;
    poly->r1 = color[1].r;
    poly->g1 = color[1].g;
    poly->b1 = color[1].b;
    poly->r2 = color[2].r;
    poly->g2 = color[2].g;
    poly->b2 = color[2].b;
    poly->r3 = color[3].r;
    poly->g3 = color[3].g;
    poly->b3 = color[3].b;
    addPrim(&g_bunit_gfx_context->otag[idx], poly);
}
