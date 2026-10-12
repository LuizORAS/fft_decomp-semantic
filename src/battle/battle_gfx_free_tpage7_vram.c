#include "fft/battle_gfx.h"
#include "fft/battle_thread.h"

/*
 * Release a texture rectangle's occupied cells in the 16-by-15 grid.
 *
 * Coordinates and dimensions round up to 16-pixel cells. The target reads
 * rect->x before checking the -1 pointer sentinel; this is not a safe invalid
 * pointer check. Out-of-range positive cell coordinates stop this thread.
 *
 * BATTLE twin of world_gfx_free_tpage7_vram.
 */
void battle_gfx_free_tpage7_vram(RECT* rect) {
    s32 x;
    s32 y;
    if (rect->x != -1 && rect != (void*)-1) {
        g_battle_gfx_tpage7_free_rect_pixels[0] = (rect->x - 0x1c0) * 4;
        g_battle_gfx_tpage7_free_rect_pixels[1] = rect->y & 0xff;
        g_battle_gfx_tpage7_free_rect_pixels[2] = rect->w * 4;
        g_battle_gfx_tpage7_free_rect_pixels[3] = rect->h;
        texture_rect_pixels_to_cells(g_battle_gfx_tpage7_free_rect_pixels, g_battle_gfx_tpage7_free_rect_cells);
        for (y = g_battle_gfx_tpage7_free_rect_cells[1];
            y < g_battle_gfx_tpage7_free_rect_cells[1] + g_battle_gfx_tpage7_free_rect_cells[3]; y++) {
            for (x = g_battle_gfx_tpage7_free_rect_cells[0];
                x < g_battle_gfx_tpage7_free_rect_cells[0] + g_battle_gfx_tpage7_free_rect_cells[2]; x++) {
                if (x >= 16 || y >= 15) {
                    battle_thread_exit_current();
                }
                g_battle_gfx_tpage7_vram_allocation_grid[y][x] = 0;
            }
        }
    }
}
