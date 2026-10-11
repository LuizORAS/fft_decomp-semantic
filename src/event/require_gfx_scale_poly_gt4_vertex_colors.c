#include "fft/event_require.h"

/* Scale each vertex color by scale / 128. */
void require_gfx_scale_poly_gt4_vertex_colors(
    const require_display_condition_colors_t* colors, s32 scale, POLY_GT4* poly) {
    s32 r;
    s32 g;
    s32 b;

    r = colors->vertices[0].r * scale / 128;
    g = colors->vertices[0].g * scale / 128;
    b = colors->vertices[0].b * scale / 128;
    poly->r0 = r;
    poly->g0 = g;
    poly->b0 = b;
    r = colors->vertices[1].r * scale / 128;
    g = colors->vertices[1].g * scale / 128;
    b = colors->vertices[1].b * scale / 128;
    poly->r1 = r;
    poly->g1 = g;
    poly->b1 = b;
    r = colors->vertices[2].r * scale / 128;
    g = colors->vertices[2].g * scale / 128;
    b = colors->vertices[2].b * scale / 128;
    poly->r2 = r;
    poly->g2 = g;
    poly->b2 = b;
    r = colors->vertices[3].r * scale / 128;
    g = colors->vertices[3].g * scale / 128;
    b = colors->vertices[3].b * scale / 128;
    poly->r3 = r;
    poly->g3 = g;
    poly->b3 = b;
}
