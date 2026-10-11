#include "fft/event_etc.h"
#include "psx/types.h"

/* The record for one half of the title: `dimension_set` picks the pair,
 * `column` the half. */
#define DIM (dimensions + dimension_set)[column]

/* Build the 2x2 POLY_GT4 grid that shows a chapter title graphic. Row 0 is
 * the part already revealed by `fade`; row 1 is the 32-pixel gradient edge. */
void etc_graphic_build_chapter_polygons(
    s32 graphic_id, s32 fade, s32 dimension_set, s32 layer, POLY_GT4* primitives, s32 color) {
    etc_graphic_dimensions_t* dimensions;
    s32 texture_mode;
    POLY_GT4* poly;
    s32 row;
    s32 column;
    s32 clip;

    dimensions = g_etc_graphics[graphic_id].dimensions;
    texture_mode = g_etc_graphics[graphic_id].texture_mode;
    poly = primitives;
    for (row = 0; row < 2; row++) {
        for (column = 0; column < 2; column++) {
            SetPolyGT4(poly);
            SetShadeTex(poly, 0);
            SetSemiTrans(poly, 1);
            setUV4(poly, 0, 0, 1, 0, 0, 1, 1, 1);
            setXY4(poly, 0, 0, 0, 0, 0, 0, 0, 0);
            if (column != 0) {
                poly->tpage = getTPage(texture_mode, 1, 0x180, 0);
            } else {
                poly->tpage = getTPage(texture_mode, 2, 0x180, 0);
            }
            if (texture_mode == 0) {
                poly->clut = 0xFDB;
            } else {
                poly->clut = 0x7F80;
            }
            if (row == 0) {
                if (layer != 0) {
                    setUV4(poly, DIM.u + DIM.width - fade, DIM.v, DIM.u + DIM.width, DIM.v, DIM.u + DIM.width - fade,
                        DIM.v + DIM.height, DIM.u + DIM.width, DIM.v + DIM.height);
                    setXY4(poly, DIM.x + DIM.width - fade + 0x100, DIM.y + 0x80, DIM.x + DIM.width + 0x100,
                        DIM.y + 0x80, DIM.x + DIM.width - fade + 0x100, DIM.y + 0x80 + DIM.height,
                        DIM.x + DIM.width + 0x100, DIM.y + 0x80 + DIM.height);
                    setRGB0(poly, color, color, color);
                    setRGB1(poly, color, color, color);
                    setRGB2(poly, color, color, color);
                    setRGB3(poly, color, color, color);
                } else {
                    setUVWH(poly, DIM.u, DIM.v, fade, DIM.height);
                    setXYWH(poly, DIM.x + 0x100, DIM.y + 0x80, fade, DIM.height);
                    setRGB0(poly, color, color, color);
                    setRGB1(poly, color, color, color);
                    setRGB2(poly, color, color, color);
                    setRGB3(poly, color, color, color);
                }
            } else if (row == 1) {
                clip = (DIM.width < fade + 0x20) ? DIM.width - fade : 0x20;
                if (layer != 0) {
                    setUV4(poly, DIM.u + DIM.width - fade - clip, DIM.v, DIM.u + DIM.width - fade, DIM.v,
                        DIM.u + DIM.width - fade - clip, DIM.v + DIM.height, DIM.u + DIM.width - fade,
                        DIM.v + DIM.height);
                    setXYWH(poly, DIM.x + DIM.width - fade + 0x100 - clip, DIM.y + 0x80, clip, DIM.height);
                    setRGB0(poly, 0, 0, 0);
                    setRGB1(poly, color, color, color);
                    setRGB2(poly, 0, 0, 0);
                    setRGB3(poly, color, color, color);
                } else {
                    setUVWH(poly, DIM.u + fade, DIM.v, clip, DIM.height);
                    setXYWH(poly, DIM.x + fade + 0x100, DIM.y + 0x80, clip, DIM.height);
                    setRGB0(poly, color, color, color);
                    setRGB1(poly, 0, 0, 0);
                    setRGB2(poly, color, color, color);
                    setRGB3(poly, 0, 0, 0);
                }
            }
            poly++;
        }
    }
    if (layer != 0) {
        battle_gfx_draw_or_append_gpu_primitive(&primitives[1]);
        battle_gfx_draw_or_append_gpu_primitive(&primitives[0]);
        battle_gfx_draw_or_append_gpu_primitive(&primitives[3]);
        battle_gfx_draw_or_append_gpu_primitive(&primitives[2]);
    } else {
        battle_gfx_draw_or_append_gpu_primitive(&primitives[3]);
        battle_gfx_draw_or_append_gpu_primitive(&primitives[2]);
        battle_gfx_draw_or_append_gpu_primitive(&primitives[1]);
        battle_gfx_draw_or_append_gpu_primitive(&primitives[0]);
    }
}
