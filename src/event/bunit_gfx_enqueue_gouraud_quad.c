#include "fft/event_bunit.h"
#include "psx/types.h"

/*
 * Append a four-color menu quad to the selected ordering-table entry.
 *
 * `pts` holds the four corners as x, y pairs and `color` four RGB triples.
 * Screen x coordinates use the menu's +0x80 origin.
 */
void bunit_gfx_enqueue_gouraud_quad(u16* pts, u8* color, s32 semi, s32 idx) {
    POLY_G4* poly;

    poly = &g_bunit_gfx_context->gouraud_quads[g_bunit_gfx_poly_g4_count++];
    SetSemiTrans(poly, semi);
    poly->x0 = pts[0] + 0x80;
    poly->y0 = pts[1];
    poly->x1 = pts[2] + 0x80;
    poly->y1 = pts[3];
    poly->x2 = pts[4] + 0x80;
    poly->y2 = pts[5];
    poly->x3 = pts[6] + 0x80;
    poly->y3 = pts[7];
    poly->r0 = color[0];
    poly->g0 = color[1];
    poly->b0 = color[2];
    poly->r1 = color[3];
    poly->g1 = color[4];
    poly->b1 = color[5];
    poly->r2 = color[6];
    poly->g2 = color[7];
    poly->b2 = color[8];
    poly->r3 = color[9];
    poly->g3 = color[10];
    poly->b3 = color[11];
    addPrim(&g_bunit_gfx_context->otag[idx], poly);
}
