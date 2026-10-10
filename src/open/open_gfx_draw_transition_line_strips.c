#include "fft/open.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Draw the text lines of the current new-game prologue entry at the given
 * brightness.
 *
 * Each step of the entry is one line of text: step_values holds its 16-pixel row
 * in the texture page and step_durations its width in pixels. The lines are
 * centred, 20 pixels apart, as a block around the middle of the screen. A
 * finished line is drawn at full width and the line in progress up to timer
 * pixels; the pass with i == step adds that line's leading edge, at most 16
 * pixels whose right-hand corners are black, so the text fades in as it is
 * revealed.
 *
 * Every piece is two POLY_GT4s: the text, added to the picture at intensity in
 * ordering-table entry mode, and a copy one pixel down and right, subtracted at
 * half intensity in entry mode + 1.
 *
 * The coordinates are written in setXY4 order (x0, y0, x1, y1, ...) per
 * primitive, and width + 1 goes through its own variable (the target adds left
 * afterwards). */
void open_gfx_draw_transition_line_strips(s32 intensity) {
    POLY_GT4* prims;
    s32 i;
    s32 prim_index;
    s32 width;
    s32 width_plus_one;
    s32 left;
    s32 top_y;
    s32 u;
    s32 v;
    u16 clut;

    for (i = 0; i < g_open_gfx_transition.step + 1; i++) {
        prim_index = i * 2;
        prims = g_open_gfx_transition_line_primitives[g_active_graphics_buffer_index];
        SetPolyGT4(&prims[prim_index]);
        SetPolyGT4(&prims[prim_index + 1]);

        if (i == g_open_gfx_transition.step) {
            /* Leading edge of the line in progress (line i - 1): black on its right. */
            if (g_open_gfx_transition.step_durations[i - 1] < g_open_gfx_transition.timer) {
                continue;
            }
            width = g_open_gfx_transition.step_durations[i - 1] - g_open_gfx_transition.timer;
            if (g_open_gfx_transition.timer < 0) {
                width = g_open_gfx_transition.timer + 0x10;
            } else if (width >= 0x11) {
                width = 0x10;
            }
            u = g_open_gfx_transition.timer;
            if (u < 0) {
                u = 0;
            }
            v = g_open_gfx_transition.step_values[i - 1] * 0x10;
            prims[prim_index].r1 = 0;
            prims[prim_index].g1 = 0;
            prims[prim_index].b1 = 0;
            prims[prim_index + 1].r1 = 0;
            prims[prim_index + 1].g1 = 0;
            prims[prim_index + 1].b1 = 0;
            prims[prim_index].r3 = 0;
            prims[prim_index].g3 = 0;
            prims[prim_index].b3 = 0;
            prims[prim_index + 1].r3 = 0;
            prims[prim_index + 1].g3 = 0;
            prims[prim_index + 1].b3 = 0;
            left = u - g_open_gfx_transition.step_durations[i - 1] / 2;
            top_y = (i - 1) * 0x14 - g_open_gfx_transition.step_count * 10;
        } else {
            /* Line i: the revealed part of the line in progress, or a whole finished line. */
            if (i == g_open_gfx_transition.step - 1) {
                width = g_open_gfx_transition.timer;
            } else {
                width = g_open_gfx_transition.step_durations[i];
            }
            u = 0;
            if (width <= 0) {
                continue;
            }
            v = g_open_gfx_transition.step_values[i] * 0x10;
            prims[prim_index].r1 = intensity;
            prims[prim_index].g1 = intensity;
            prims[prim_index].b1 = intensity;
            prims[prim_index + 1].r1 = intensity / 2;
            prims[prim_index + 1].g1 = intensity / 2;
            prims[prim_index + 1].b1 = intensity / 2;
            prims[prim_index].r3 = intensity;
            prims[prim_index].g3 = intensity;
            prims[prim_index].b3 = intensity;
            prims[prim_index + 1].r3 = intensity / 2;
            prims[prim_index + 1].g3 = intensity / 2;
            prims[prim_index + 1].b3 = intensity / 2;
            left = -(g_open_gfx_transition.step_durations[i] / 2);
            top_y = i * 0x14 - g_open_gfx_transition.step_count * 10;
        }

        prims[prim_index].r0 = intensity;
        prims[prim_index].g0 = intensity;
        prims[prim_index].b0 = intensity;
        prims[prim_index + 1].r0 = intensity / 2;
        prims[prim_index + 1].g0 = intensity / 2;
        prims[prim_index + 1].b0 = intensity / 2;
        prims[prim_index].r2 = intensity;
        prims[prim_index].g2 = intensity;
        prims[prim_index].b2 = intensity;
        prims[prim_index + 1].r2 = intensity / 2;
        prims[prim_index + 1].g2 = intensity / 2;
        prims[prim_index + 1].b2 = intensity / 2;

        clut = GetClut(0, 0x1e3);
        prims[prim_index + 1].clut = clut;
        prims[prim_index].clut = clut;
        prims[prim_index].tpage = GetTPage(0, 1, g_open_gfx_transition.tpage_x, g_open_gfx_transition.tpage_y);
        prims[prim_index + 1].tpage = GetTPage(0, 2, g_open_gfx_transition.tpage_x, g_open_gfx_transition.tpage_y);

        prims[prim_index].x0 = left;
        prims[prim_index].y0 = top_y;
        prims[prim_index].x1 = left + width;
        prims[prim_index].y1 = top_y;
        prims[prim_index].x2 = left;
        prims[prim_index].y2 = top_y + 0x10;
        prims[prim_index].x3 = left + width;
        prims[prim_index].y3 = top_y + 0x10;
        width_plus_one = width + 1;
        prims[prim_index + 1].x0 = left + 1;
        prims[prim_index + 1].y0 = top_y + 1;
        prims[prim_index + 1].x1 = left + width_plus_one;
        prims[prim_index + 1].y1 = top_y + 1;
        prims[prim_index + 1].x2 = left + 1;
        prims[prim_index + 1].y2 = top_y + 0x11;
        prims[prim_index + 1].x3 = left + width_plus_one;
        prims[prim_index + 1].y3 = top_y + 0x11;
        prims[prim_index].u0 = u;
        prims[prim_index].v0 = v;
        prims[prim_index].u1 = u + width;
        prims[prim_index].v1 = v;
        prims[prim_index].u2 = u;
        prims[prim_index].v2 = v + 0x10;
        prims[prim_index].u3 = u + width;
        prims[prim_index].v3 = v + 0x10;
        prims[prim_index + 1].u0 = u;
        prims[prim_index + 1].v0 = v;
        prims[prim_index + 1].u1 = u + width;
        prims[prim_index + 1].v1 = v;
        prims[prim_index + 1].u2 = u;
        prims[prim_index + 1].v2 = v + 0x10;
        prims[prim_index + 1].u3 = u + width;
        prims[prim_index + 1].v3 = v + 0x10;

        SetSemiTrans(&prims[prim_index], 1);
        SetSemiTrans(&prims[prim_index + 1], 1);
        AddPrim(&g_open_gfx_otags[g_active_graphics_buffer_index][g_open_gfx_transition.mode], &prims[prim_index]);
        AddPrim(
            &g_open_gfx_otags[g_active_graphics_buffer_index][g_open_gfx_transition.mode + 1], &prims[prim_index + 1]);
    }
}
