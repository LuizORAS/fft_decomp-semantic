#include "fft/event_equip.h"
#include "psx/types.h"

/*
 * Append a textured menu quad, given by its four corners, to the selected
 * ordering-table entry.
 *
 * Screen x coordinates use the menu's +0x80 origin. With `color` the quad is
 * shaded with that color; otherwise texture shading is disabled.
 */
void equip_gfx_enqueue_textured_quad_corners(
    const world_gfx_textured_quad_source_t* descriptor, const rgb24_t* color, s32 semitrans, s32 otag_index) {
    s32 quad_semitrans = semitrans;
    s32 quad_otag = otag_index;
    POLY_FT4* poly;

    poly = &g_equip_gfx_context->textured_quads[g_equip_gfx_poly_ft4_count++];

    if (color != 0) {
        SetShadeTex(poly, 0);
        setRGB0(poly, color->r, color->g, color->b);
    } else {
        SetShadeTex(poly, 1);
    }
    SetSemiTrans(poly, quad_semitrans);

    setXY4(poly, descriptor->x0 + 0x80, descriptor->y0, descriptor->x1 + 0x80, descriptor->y1, descriptor->x2 + 0x80,
        descriptor->y2, descriptor->x3 + 0x80, descriptor->y3);

    setUV4(poly, descriptor->u0, descriptor->v0, descriptor->u1, descriptor->v1, descriptor->u2, descriptor->v2,
        descriptor->u3, descriptor->v3);
    poly->clut = descriptor->clut;
    poly->tpage = descriptor->tpage;

    addPrim(&g_equip_gfx_context->otag[quad_otag], poly);
}
