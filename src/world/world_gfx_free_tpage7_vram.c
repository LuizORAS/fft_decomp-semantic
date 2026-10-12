#include "fft/world.h"

/*
 * Release a texture rectangle's occupied cells in the 16-by-15 grid.
 *
 * Coordinates and dimensions round up to 16-pixel cells. The target reads
 * rect->x before checking the -1 pointer sentinel; this is not a safe invalid
 * pointer check. Out-of-range positive cell coordinates stop this thread.
 */
void world_gfx_free_tpage7_vram(RECT* rect) {
    s32 x;
    s32 y;
    if (rect->x != -1 && rect != (void*)-1) {
        g_world_gfx_free_rect_texels[0] = (rect->x - 0x1c0) * 4;
        g_world_gfx_free_rect_texels[1] = rect->y & 0xFF;
        g_world_gfx_free_rect_texels[2] = rect->w * 4;
        g_world_gfx_free_rect_texels[3] = rect->h;
        texture_rect_pixels_to_cells(g_world_gfx_free_rect_texels, g_world_gfx_free_rect_grid_cells);
        for (y = g_world_gfx_free_rect_grid_cells[1];
            y < g_world_gfx_free_rect_grid_cells[1] + g_world_gfx_free_rect_grid_cells[3]; y++) {
            for (x = g_world_gfx_free_rect_grid_cells[0];
                x < g_world_gfx_free_rect_grid_cells[0] + g_world_gfx_free_rect_grid_cells[2]; x++) {
                if (x >= WORLD_TPAGE7_GRID_COLUMNS || y >= WORLD_TPAGE7_GRID_ROWS) {
                    world_thread_exit_current();
                }
                g_world_gfx_texture_allocation_grid[y][x] = 0;
            }
        }
    }
}
