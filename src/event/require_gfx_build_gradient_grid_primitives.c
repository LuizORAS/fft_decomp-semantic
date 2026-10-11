#include "fft/event_require.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Build the 4x8 grid of shaded, textured quads behind the panel: each row is
 * one step brighter than the one above, and every cell shows the same 64x32
 * texture rectangle. */
void require_gfx_build_gradient_grid_primitives(POLY_GT4* poly) {
    s32 row;
    s32 column;
    s32 intensity;

    for (row = 0; row < 8; row++) {
        for (column = 0; column < 4; column++) {
            SetPolyGT4(poly);
            poly->clut = 0x7fbc;
            poly->tpage = GetTPage(0, 0, 0x3c0, 0x100);
            intensity = row * 8 + 0x40;
            setRGB0(poly, intensity, intensity, intensity);
            setRGB1(poly, intensity, intensity, intensity);
            intensity = (row + 1) * 8 + 0x40;
            setRGB2(poly, intensity, intensity, intensity);
            setRGB3(poly, intensity, intensity, intensity);
            poly->x0 = column * 0x40 + 0x80;
            poly->y0 = row * 0x20;
            poly->x1 = column * 0x40 + 0xc0;
            poly->y1 = row * 0x20;
            poly->x2 = column * 0x40 + 0x80;
            poly->y2 = row * 0x20 + 0x20;
            poly->x3 = column * 0x40 + 0xc0;
            poly->y3 = row * 0x20 + 0x20;
            poly->u0 = 0x58;
            poly->v0 = 0xd8;
            poly->u1 = 0x98;
            poly->v1 = 0xd8;
            poly->u2 = 0x58;
            poly->v2 = 0xf8;
            poly->u3 = 0x98;
            poly->v3 = 0xf8;
            SetSemiTrans(poly, 1);
            SetShadeTex(poly, 0);
            battle_gfx_append_gpu_primitive_to_secondary_otag((u32*)poly);
            poly++;
        }
    }
}
