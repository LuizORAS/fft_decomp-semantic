#include "fft/gfx.h"
#include "fft/thread.h"

/* Allocate a texture rectangle in the 16-column VRAM allocation grid.
 *
 * The rect size rounds up to 16-pixel cells. The first free position whose
 * cells are all unclaimed is marked 0xff, the primitive UVs/CLUT (and TPage
 * for POLY_FT4) are pointed at it, and rect is rewritten to the 4bpp VRAM
 * area at x 0x1c0 before image (unless -1) is uploaded. A full grid stops
 * this thread. Each module that compiles this text (BATTLE; WORLD as world_gfx_alloc_tpage7_vram) binds
 * its own grid, which the module's free_tpage7_vram routine releases. */
void gfx_alloc_tpage7_vram(RECT* rect, void* prim, u32* image) {
    s16 width;
    s16 height;
    s32 width_cells;
    s32 height_cells;
    s32 row;
    s32 column;
    s32 y;
    s32 x;

    width = rect->w;
    height = rect->h;
    width_cells = width >> 4;
    height_cells = height >> 4;
    if (width & 0xF) {
        width_cells++;
    }
    if (height & 0xF) {
        height_cells++;
    }
    for (row = 0; row < 16 - height_cells; row++) {
        for (column = 0; column <= 16 - width_cells; column++) {
            if (g_gfx_tpage7_vram_allocation_grid[row][column] != 0) {
                continue;
            }
            for (y = row; y < row + height_cells; y++) {
                for (x = column; x < column + width_cells; x++) {
                    if (g_gfx_tpage7_vram_allocation_grid[y][x] == 0xFF) {
                        goto next;
                    }
                }
            }
            for (y = row; y < row + height_cells; y++) {
                for (x = column; x < column + width_cells; x++) {
                    g_gfx_tpage7_vram_allocation_grid[y][x] = 0xFF;
                }
            }
            x = column * 16;
            y = row * 16;
            if ((getcode(prim) & 0xFC) == 0x64) {
                SPRT* sprite = prim;

                sprite->u0 = x;
                sprite->v0 = y;
                sprite->w = rect->w;
                sprite->h = rect->h;
                sprite->clut = 0x7C3C;
            } else {
                POLY_FT4* quad = prim;

                quad->u0 = quad->u2 = x;
                quad->v0 = quad->v1 = y;
                if (x + rect->w >= 0x100) {
                    x--;
                }
                quad->u1 = quad->u3 = rect->w + x;
                quad->v2 = quad->v3 = rect->h + y;
                quad->tpage = GetTPage(0, 0, 0x1C0, 0);
                quad->clut = 0x7C3C;
            }
            rect->x = column * 4 + 0x1C0;
            rect->y = y;
            rect->w = (rect->w & 3) ? (rect->w >> 2) + 1 : rect->w >> 2;
            if (image != (u32*)-1) {
                LoadImage(rect, image);
            }
            return;
        next:;
        }
    }
    thread_exit_current();
}
