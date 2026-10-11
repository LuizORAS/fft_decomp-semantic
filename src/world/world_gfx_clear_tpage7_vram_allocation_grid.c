#include "fft/world.h"
#include "psx/types.h"

/* Clear the 15-by-16 allocation grid consumed by the tpage-7 VRAM allocator. */
void world_gfx_clear_tpage7_vram_allocation_grid(void) {
    s32 row;
    s32 column;

    for (row = 0; row < WORLD_TPAGE7_GRID_ROWS; row++) {
        for (column = WORLD_TPAGE7_GRID_COLUMNS - 1; column >= 0; column--) {
            g_world_gfx_texture_allocation_grid[row][column] = 0;
        }
    }
}
